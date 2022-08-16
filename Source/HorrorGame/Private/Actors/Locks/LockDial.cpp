// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Locks/LockDial.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALockDial::ALockDial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	LockDialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerMesh"));
	LockDialMesh->SetupAttachment(RootComp);
	LockDialMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	Index = 0;
	Number = 0;
	DialRotation = 0.f;
	bCanRotate = true;
}

// Called when the game starts or when spawned
void ALockDial::BeginPlay()
{
	Super::BeginPlay();
	
	//FindNumber(); // uncomment this if removing the SetRandomNumber() call below (There is a FindNumber() call in that function)
	SetRandomNumber();


	// Setup timeline function calls
	if (CurveFloat)
	{
		// Sets event to call when interpolating rotation
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ALockDial::InterpDialRotation);
		RotateDialTL.AddInterpFloat(CurveFloat, TimelineProgress);

		// Sets event to call when timeline finishes.
		FOnTimelineEvent TimelineEvent;
		TimelineEvent.BindUFunction(this, FName(TEXT("TimelineFinished")));
		RotateDialTL.SetTimelineFinishedFunc(TimelineEvent);
	}

	/*
	//debug
	FTimerHandle DebugTimer;
	GetWorld()->GetTimerManager().SetTimer(DebugTimer, this, &ALockDial::RotateDial, 2.f, true, 10.f);
	*/
}

// Called every frame
void ALockDial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateDialTL.TickTimeline(DeltaTime);

}

void ALockDial::TimelineFinished()
{

	// Checks if Roll rotation 
	//DialRotation = LockDialMesh->GetComponentRotation().Roll;
	//FRotator NewDialRotation = LockDialMesh->GetComponentRotation();
	//NewDialRotation.Roll = DialRotation;

	/*
	if (DialRotation >= 360.f)
	{
		DialRotation = 0.f;
		NewDialRotation.Roll = DialRotation;
		LockDialMesh->SetRelativeRotation(FRotator(NewDialRotation));
	}
	*/

	DialRotation = LockDialMesh->GetComponentRotation().Roll;
	if (DialRotation < 0.f) // Correcting gimbal lock
	{
		DialRotation = 180 + (180 - FMath::Abs(DialRotation));
	}

	if (DialRotation >= 360.f)
	{
		FRotator NewDialRotation = LockDialMesh->GetComponentRotation();
		NewDialRotation.Roll = 0.f;
		DialRotation = 0.f;
		LockDialMesh->SetRelativeRotation(FRotator(NewDialRotation));
	}


	FindNumber();
	bCanRotate = true;
}

void ALockDial::SetRandomNumber()
{
	float LocalRotation = UKismetMathLibrary::RandomIntegerInRange(0, 9) * 36.f;
	FRotator NewRotation = FRotator(LockDialMesh->GetComponentRotation().Pitch, LockDialMesh->GetComponentRotation().Yaw, LocalRotation);

	LockDialMesh->SetRelativeRotation(NewRotation);
	DialRotation = LocalRotation;
	FindNumber();
}

void ALockDial::FindNumber()
{
	Number = DialRotation / 36.f;
	UE_LOG(LogTemp, Error, TEXT("ALockDial::FindNumber %d And rotation is %f"), Number, DialRotation);
}

void ALockDial::RotateDial()
{
	if (bCanRotate)
	{
		UE_LOG(LogTemp, Error, TEXT("ALockDial::RotateDial ROTATING DIAL"));
		bCanRotate = false;
		RotateDialTL.PlayFromStart();
	}
}

void ALockDial::InterpDialRotation(float Value)
{
	/*
	float NewRollRotation = DialRotation + UKismetMathLibrary::Lerp(0.f, 36.f, Value); // 180 
	FRotator NewDialRotation = LockDialMesh->GetComponentRotation();
	NewDialRotation.Roll = NewRollRotation;
	
	LockDialMesh->SetRelativeRotation(FRotator(NewDialRotation));
	*/

	
	float Angle = 36.f;
	float NewRollRotation = DialRotation + (Angle * Value);

	// Sets new rotation
	FRotator NewDialRotation = FRotator(LockDialMesh->GetComponentRotation().Pitch, LockDialMesh->GetComponentRotation().Yaw, NewRollRotation);
	LockDialMesh->SetRelativeRotation(NewDialRotation);
}

