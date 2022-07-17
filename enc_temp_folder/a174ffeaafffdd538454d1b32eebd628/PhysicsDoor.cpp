// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabables/PhysicsDoor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
//#include "PhysicsEngine/BodyInstance.h"
#include "Characters/HGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"

// Sets default values
APhysicsDoor::APhysicsDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DoorFrame);
	Door->SetRelativeLocation(FVector(0.f, 45.f, 0.f));
	Door->SetSimulatePhysics(true);
	Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	Door->SetMassOverrideInKg(NAME_None, 150.f);
	Door->SetAngularDamping(4.f); // so the door does not swing too easily.
	// Locks constraints
	Door->BodyInstance.bLockXTranslation = true;
	Door->BodyInstance.bLockYTranslation = true;
	Door->BodyInstance.bLockZTranslation = true;
	Door->BodyInstance.bLockXRotation = true;
	Door->BodyInstance.bLockYRotation = true;

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	PhysicsConstraint->SetRelativeLocation(FVector(0.f, 45.f, 0.f));
	PhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 90.f);
	PhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.f);
	PhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.f);
	PhysicsConstraint->ComponentName1.ComponentName = FName(TEXT("DoorFrame"));
	PhysicsConstraint->ComponentName2.ComponentName = FName(TEXT("Door"));
	PhysicsConstraint->SetDisableCollision(true);
	// ?
	//PhysicsConstraint->
	DoorOpeningSensitivity = 0.5f;
}

// Called when the game starts or when spawned
void APhysicsDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicsDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APhysicsDoor::MoveDoor(float Amount)
{
	// inverts the rotation of the door according this door forward vector and actor forward vector
	float InvertMultiplier = 0;

	if (GrabberPlayer)
	{
		// Get InteractingPlayer and this door forward vectors.
		FVector CharacterFV = GrabberPlayer->GetActorForwardVector();
		FVector DoorFV = GetActorForwardVector();

		// Dot product to determine if the player is facing the same forward vector as the door.
		InvertMultiplier = (FVector::DotProduct(CharacterFV, DoorFV) >= 0.f) ? 1.f : -1.f;
		Amount = Amount * InvertMultiplier * DoorOpeningSensitivity;
	}

	FRotator RotationAmount = FRotator(0.f, Amount, 0.f);
	FRotator NewDoorRotation = Door->GetRelativeRotation() + RotationAmount;

	Door->SetRelativeRotation(NewDoorRotation);
}

void APhysicsDoor::GrabObject()
{
	if (GrabberPlayer)
	{
		// Disable ability to move and look around
		GrabberPlayer->GetCharacterMovement()->DisableMovement();
		GrabberPlayer->GetController()->SetIgnoreLookInput(true);
		//GrabberPlayer->EnableInput(Cast<APlayerController>(GrabberPlayer->GetController()));
		EnableInput(Cast<APlayerController>(GrabberPlayer->GetController()));

		UInputComponent* GrabberPlayerInputComp = GrabberPlayer->InputComponent;
		if (GrabberPlayerInputComp)
		{
			GrabberPlayerInputComp->BindAxis(TEXT("LookRight"), this, &APhysicsDoor::MoveDoor);
		}
	}
}

void APhysicsDoor::ReleaseObject()
{
	if (GrabberPlayer)
	{
		GrabberPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
		GrabberPlayer->GetController()->ResetIgnoreLookInput();
		DisableInput(Cast<APlayerController>(GrabberPlayer->GetController()));

		/*
		UInputComponent* GrabberPlayerInputComp = GrabberPlayer->InputComponent;
		if (GrabberPlayerInputComp)
		{
			GrabberPlayerInputComp->RemoveActionBinding(FName(TEXT("LookRight")), EInputEvent::IE_Pressed); // ?
		}
		*/
	}
}

