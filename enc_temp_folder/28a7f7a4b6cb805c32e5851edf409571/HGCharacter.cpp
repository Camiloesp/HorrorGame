// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HGCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Interaction.h"
#include "Interfaces/GrabInterface.h"
#include "Widgets/MainHUD.h"

//#include "Engine/World.h"

// Sets default values
AHGCharacter::AHGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	FollowCamera->bUsePawnControlRotation = true;


	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->JumpZVelocity = 400.f;
	
}

// Called when the game starts or when spawned
void AHGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
}

// Called every frame
void AHGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* BIND AXIS */
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AHGCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AHGCharacter::LookRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHGCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHGCharacter::MoveRight);

	/* BIND ACTIONS */
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Action"), EInputEvent::IE_Pressed, this, &AHGCharacter::ActionButtonPressed);

	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), EInputEvent::IE_Pressed, this, &AHGCharacter::LeftMouseButtonPressed);
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), EInputEvent::IE_Released, this, &AHGCharacter::LeftMouseButtonReleased);
}

void AHGCharacter::LookUp(float Value)
{
	AddControllerPitchInput(TurnRate * Value);
}

void AHGCharacter::LookRight(float Value)
{
	AddControllerYawInput(TurnRate * Value);
}

void AHGCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AHGCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AHGCharacter::ActionButtonPressed()
{
	// Interacts with the actor, if is a child of the IInteraction.
	IInteraction* InteractingActor = Cast<IInteraction>(LineTrace(DistanceToInteract));
	if (InteractingActor)
	{
		// Sets reference to the player interacting with the interactable and then executing Interaction logic.
		InteractingActor->InteractingPlayer = this;
		InteractingActor->Interact();
	}
}

void AHGCharacter::LeftMouseButtonPressed()
{
	GrabbedActor = Cast<IGrabInterface>(LineTrace(DistanceToInteract));
	if (GrabbedActor)
	{
		GrabbedActor->GrabberPlayer = this;
		GrabbedActor->GrabObject();
	}
}

void AHGCharacter::LeftMouseButtonReleased()
{
	if (GrabbedActor)
	{
		GrabbedActor->ReleaseObject();
		GrabbedActor->GrabberPlayer = nullptr;
		GrabbedActor = nullptr;
	}
}

AActor* AHGCharacter::LineTrace(float Length)
{
	FHitResult OutHitResult;
	const FVector Start = FollowCamera->GetComponentLocation(); // Linetrace starts from the camera location in world space.
	const FVector End = (FollowCamera->GetForwardVector() * Length) + (FollowCamera->GetComponentLocation()); // Linetrace ends Length units in front of player.
	 
	GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);

	return OutHitResult.GetActor();
}

void AHGCharacter::Initialize()
{
	// Variables Initialization
	TurnRate = 0.4;
	DistanceToInteract = 350.f;

	//Create widget, and add it to viewport.
	if (PlayerHUDClass)
	{
		MainHUDRef = CreateWidget<UMainHUD>(GetWorld(), PlayerHUDClass);
		if (MainHUDRef)
		{
			MainHUDRef->AddToViewport();
		}
	}
}
