// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Locks/LockDial.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "Widgets/ArrowWidget.h"
#include "Actors/Locks/Lock.h"

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

	UpArrow = CreateDefaultSubobject<UWidgetComponent>(TEXT("UpArrowIncrement"));
	UpArrow->SetupAttachment(RootComp);
	UpArrow->SetRelativeScale3D(FVector(0.001f, 0.001f, 0.001f));
	UpArrow->SetRelativeLocation(FVector(1.f, 0.f, 1.f));
	UpArrow->SetRelativeRotation(FRotator(0.f, 0.f, 180.f));
	UpArrow->SetVisibility(false);

	DownArrow = CreateDefaultSubobject<UWidgetComponent>(TEXT("DownArrowIncrement"));
	DownArrow->SetupAttachment(RootComp);
	DownArrow->SetRelativeScale3D(FVector(0.001f, 0.001f, 0.001f));
	DownArrow->SetRelativeLocation(FVector(1.f, 0.f, -1.f));
	DownArrow->SetVisibility(false);

	//DownArrow
	Index = 0;
	Number = 0;
	DialRotation = 0.f;
	bCanRotate = true;
}

// Called when the game starts or when spawned
void ALockDial::BeginPlay()
{
	Super::BeginPlay();

	// Set reference to this class in our widget
	UArrowWidget* UpWidget = Cast<UArrowWidget>(UpArrow->GetUserWidgetObject());
	UArrowWidget* DownWidget = Cast<UArrowWidget>(DownArrow->GetUserWidgetObject());
	if (UpWidget && DownWidget)
	{
		UpWidget->SetLockDialRef(this);
		DownWidget->SetLockDialRef(this);

		UpWidget->SetAddRotation(true);
		DownWidget->SetAddRotation(false);
	}


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

	// Check if this rotation unlocked the lock.
	if (!LockRef) return;
	LockRef->CheckEveryTurn();
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

void ALockDial::FindNumber()
{
	Number = FMath::Abs(DialRotation / 36.f);
}

void ALockDial::RotateDial()
{
	if (bCanRotate)
	{
		bAddRotation = true;
		bCanRotate = false;
		RotateDialTL.PlayFromStart();
	}
}

void ALockDial::RotateDialBackwards()
{
	if (bCanRotate)
	{
		bAddRotation = false;
		bCanRotate = false;
		RotateDialTL.ReverseFromEnd();
	}
}

void ALockDial::InterpDialRotation(float Value)
{
	float NewValue = bAddRotation ? Value : (1 - Value); // When reversing timeline, value should always start from 0 and end in 1.
	float Angle = bAddRotation ? 36.f : -36.f;
	float RotationTarget = DialRotation + Angle;

	float NewRollRotation = DialRotation + (Angle * NewValue);
	FRotator NewDialRotation = LockDialMesh->GetRelativeRotation();
	NewDialRotation.Roll = NewRollRotation;
	LockDialMesh->SetRelativeRotation(NewDialRotation);

	//UE_LOG(LogTemp, Error, TEXT("Angle: %f, CurrentRotation: %f, RotationTarget: %f, NewRollRotation: %f, Value: %f"), Angle, DialRotation, RotationTarget, NewRollRotation, Value);
	//UE_LOG(LogTemp, Error, TEXT("Angle: %f, CurrentRotation: %f, RotationTarget: %f, NewRollRotation: %f, Value: %f"), Angle, DialRotation, RotationTarget, NewRollRotation, NewValue);
}

