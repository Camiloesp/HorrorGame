// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Door.h"
//#include "Components/TimelineComponent.h"
#include "Characters/HGCharacter.h"

ADoor::ADoor()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	Door->SetupAttachment(InteractableMesh);
	Door->SetRelativeLocation(FVector(0.f, 45.f, 0.f));

	bDoorOnSameSide = false;
	bTwoSidedDoor = true;
	bIsDoorClosed = true;
	DoorRotateAngle = 90.f;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Sends delta time to our timeline to interpolate when opening door.
	Timeline.TickTimeline(DeltaTime);
}

void ADoor::OpenDoor(float Value)
{
	float Angle = bDoorOnSameSide ? -DoorRotateAngle : DoorRotateAngle;

	// Sets new rotation
	FRotator NewDoorRotation = FRotator(0.f, Angle * Value, 0.f);
	Door->SetRelativeRotation(NewDoorRotation);
}

void ADoor::SetDoorOnSameSide()
{
	if (!bTwoSidedDoor) return;

	if (InteractingPlayer)
	{
		// Get InteractingPlayer and this door forward vectors.
		FVector CharacterFV = InteractingPlayer->GetActorForwardVector();
		FVector DoorFV = GetActorForwardVector();

		// Dot product to determine if the player is facing the same forward vector as the door.
		bDoorOnSameSide = (FVector::DotProduct(CharacterFV, DoorFV) >= 0.f);
	}
}

void ADoor::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting... With a door"));
	if (bIsDoorClosed)
	{
		SetDoorOnSameSide();
		Timeline.Play(); // Open door
	}
	else
	{
		Timeline.Reverse(); // Close door
	}

	bIsDoorClosed = !bIsDoorClosed; // Flip flop

	Super::Interact();
}
