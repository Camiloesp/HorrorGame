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

	//FindNumber(); // uncomment this if removing the SetRandomNumber() call below (There is a FindNumber() call in that function)
	SetRandomNumber();
}

// Called every frame
void ALockDial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateDialTL.TickTimeline(DeltaTime);

}

void ALockDial::TimelineFinished()
{
	// Correcting gimbal lock
	DialRotation = LockDialMesh->GetRelativeRotation().Roll;
	if (DialRotation < 0.f) 
	{
		DialRotation = 180 + (180 - FMath::Abs(DialRotation));
	}

	// Set angle to 0 if >= 360
	if (DialRotation >= 360.f)
	{
		FRotator NewDialRotation = LockDialMesh->GetRelativeRotation();
		NewDialRotation.Roll = 0.f;
		DialRotation = 0.f;
		LockDialMesh->SetRelativeRotation(NewDialRotation);
	}

	FindNumber();
	bCanRotate = true;
}

void ALockDial::SetRandomNumber()
{
	float NewRoll = UKismetMathLibrary::RandomIntegerInRange(0, 9) * 36.f;
	FRotator NewRotation = LockDialMesh->GetRelativeRotation();
	NewRotation.Roll = NewRoll;

	LockDialMesh->SetRelativeRotation(NewRotation);
	DialRotation = NewRoll;

	FindNumber();
}

void ALockDial::HandleDialRotation()
{
}

void ALockDial::FindNumber()
{
	Number = DialRotation / 36.f;
}

void ALockDial::RotateDial()
{
	if (bCanRotate)
	{
		bCanRotate = false;
		RotateDialTL.PlayFromStart();
	}
}

void ALockDial::InterpDialRotation(float Value)
{
	float Angle = 36.f;
	float NewRollRotation = DialRotation + (Angle * Value);
	FRotator NewDialRotation = LockDialMesh->GetRelativeRotation();
	NewDialRotation.Roll = NewRollRotation;

	// Sets new rotation
	LockDialMesh->SetRelativeRotation(NewDialRotation);
}

