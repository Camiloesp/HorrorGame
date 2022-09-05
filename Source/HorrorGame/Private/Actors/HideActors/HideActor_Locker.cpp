// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HideActors/HideActor_Locker.h"
#include "Components/ArrowComponent.h"
#include "Characters/HGCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AHideActor_Locker::AHideActor_Locker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	LockerDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerDoorMesh"));
	LockerDoorMesh->SetupAttachment(GetMesh());
	LockerDoorMesh->SetRelativeLocation(FVector(-37.5f, 39.f, 0.f));

	GetHidingPosition()->SetRelativeLocation(FVector(0.f, 0.f, 88.f)); // 88 is the height of the player capsule

	GetExitPosition()->SetRelativeLocation(FVector(200.f, 0.f, 88.f)); // 88 is the height of the player capsule

	GetAILocation()->SetRelativeLocation(FVector(150.f, 0.f, 88.f));
	GetAILocation()->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
}

void AHideActor_Locker::BeginPlay()
{
	Super::BeginPlay();

	FinishMovingDelegate.AddDynamic(this, &AHideActor_Locker::StartClosingDoor);

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHideActor_Locker::OpenLockerDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

void AHideActor_Locker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Sends delta time to our timeline to interpolate when opening door.
	Timeline.TickTimeline(DeltaTime);
}

void AHideActor_Locker::StartClosingDoor()
{
	Timeline.Reverse();
}

void AHideActor_Locker::OpenLockerDoor(float Value)
{
	// Locker door opens 90 degrees
	float Angle = 90.f;

	// Sets new rotation
	FRotator NewDoorRotation = FRotator(0.f, Angle * Value, 0.f);
	LockerDoorMesh->SetRelativeRotation(NewDoorRotation);

	if (Timeline.GetTimelineLength() == Timeline.GetPlaybackPosition())
	{
		UE_LOG(LogTemp, Warning, TEXT("Timeline finished playing"));
		bCanInteract = true;
	}
}

void AHideActor_Locker::CallInteractParent()
{
	AHideActor::Interact();
	//Super::Interact();
}

void AHideActor_Locker::EnemyFound()
{
	// disable LockerDoorMesh collision so it doesnt push the AI
	LockerDoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// AI opens door
	bCanInteract = false;
	Timeline.Play(); // Open door
}

void AHideActor_Locker::Interact()
{
	if (!InteractingPlayer) return;
	if (!bCanInteract) return;
	
	bCanInteract = false;

	// Open the locker door
	Timeline.Play();

	FTimerHandle DelayToEnterLocker;
	GetWorld()->GetTimerManager().SetTimer(DelayToEnterLocker, this, &AHideActor_Locker::CallInteractParent, Timeline.GetTimelineLength(), false);
	//Super::Interact();
}
