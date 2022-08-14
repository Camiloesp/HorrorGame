// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HideActors/HideActor.h"
#include "Components/ArrowComponent.h"
#include "Characters/HGCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHideActor::AHideActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerMesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	HidingPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("HidingPositionArrow"));
	HidingPosition->SetupAttachment(RootComp);

	ExitPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerExitPositionArrow"));
	ExitPosition->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AHideActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHideActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHideActor::MoveCharacter()
{
	FTransform NewPlayerTransform = FTransform();

	// Interpolates player location to inside the locker
	FVector CurrentPlayerLocation = InteractingPlayer->GetActorLocation();
	FVector PlayerTargetLocation = (InteractingPlayer->IsHiding()) ? GetExitPosition()->GetComponentLocation() : GetHidingPosition()->GetComponentLocation();
	float CurrentDeltaTime = GetWorld()->DeltaTimeSeconds;
	FVector NewPlayerLocation = UKismetMathLibrary::VInterpTo_Constant(CurrentPlayerLocation, PlayerTargetLocation, CurrentDeltaTime, 1'000.f);

	// Interpolates player rotation to inside the locker.
	//TODO

	// Sets new location
	NewPlayerTransform.SetLocation(NewPlayerLocation);
	InteractingPlayer->SetActorTransform(NewPlayerTransform);

	// if we arrive to the location, stop timer.
	bool bIsInDestination = UKismetMathLibrary::EqualEqual_VectorVector(CurrentPlayerLocation, PlayerTargetLocation, 0.01);
	if (bIsInDestination)
	{
		GetWorld()->GetTimerManager().ClearTimer(HidingMovementTimer);
		FinishMoving();
	}
}

void AHideActor::FinishMoving()
{
	FinishMovingDelegate.Broadcast();

	if (InteractingPlayer->IsHiding())
	{
		InteractingPlayer->SetIsHiding(false);

		// Enable collision when exiting hidding spot.
		InteractingPlayer->SetActorEnableCollision(true);

		// Enable player movement when exiting the locker.
		InteractingPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else
	{
		InteractingPlayer->SetIsHiding(true);
	}
}

void AHideActor::Interact()
{
	if (!InteractingPlayer) return;

	if (InteractingPlayer->IsHiding())
	{
		// Set timer to move character to interpolate movement
		GetWorld()->GetTimerManager().SetTimer(HidingMovementTimer, this, &AHideActor::MoveCharacter, GetWorld()->DeltaTimeSeconds, true);
	}
	else
	{
		// disable collision when entering hidding spot.
		InteractingPlayer->SetActorEnableCollision(false);

		// Disable player movement when inside the locker.
		InteractingPlayer->GetCharacterMovement()->DisableMovement();

		// Set timer to move character to interpolate movement
		GetWorld()->GetTimerManager().SetTimer(HidingMovementTimer, this, &AHideActor::MoveCharacter, GetWorld()->DeltaTimeSeconds, true);
	}
}

