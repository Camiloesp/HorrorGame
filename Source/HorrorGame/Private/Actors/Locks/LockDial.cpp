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
	
	FindNumber();
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ALockDial::InterpDialRotation);
		RotateDialTL.AddInterpFloat(CurveFloat, TimelineProgress);
	}

	RotateDial();
}

// Called every frame
void ALockDial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateDialTL.TickTimeline(DeltaTime);

}

void ALockDial::FindNumber()
{
	Number = LockDialMesh->GetRelativeRotation().Roll / 36.f;
	//if () // 360/36 = 10.f (we set back to 0.f)
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
	//DialRotation = DialRotation + Value;
	float NewRollRotation = UKismetMathLibrary::Lerp(0.f, 36.f, Value) + DialRotation;
	FRotator NewDialRotation = LockDialMesh->GetComponentRotation();
	NewDialRotation.Roll = NewRollRotation;

	DialRotation = NewRollRotation; // ? Here

	LockDialMesh->SetRelativeRotation(FRotator(NewDialRotation));
}

