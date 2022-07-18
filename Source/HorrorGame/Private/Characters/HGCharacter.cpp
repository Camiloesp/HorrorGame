// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Interaction.h"
#include "Interfaces/GrabInterface.h"
#include "Widgets/MainHUD.h"
#include "Components/Movement.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AHGCharacter::AHGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	FollowCamera->bUsePawnControlRotation = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(FollowCamera);
	SpringArm->TargetArmLength = 10.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, -30.f);
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 10.f;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(SpringArm);
	Flashlight->AttenuationRadius = 1200.f;
	//Flashlight transform to 0.f

	HGMovementComp = CreateDefaultSubobject<UMovement>(TEXT("MovementComp"));

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

	PlayerInputComponent->BindAction(TEXT("FlashlightToggle"), EInputEvent::IE_Pressed, this, &AHGCharacter::ToggleFlashlight);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AHGCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AHGCharacter::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AHGCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &AHGCharacter::CrouchButtonReleased);
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
	HeadBob();
}

void AHGCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
	HeadBob();
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

	HGMovementComp->Initialize(this);
	
}

void AHGCharacter::ToggleFlashlight()
{
	bool bIsFlashlightOn = !( Flashlight->IsVisible() );
	Flashlight->SetVisibility(bIsFlashlightOn);
}

void AHGCharacter::Sprint()
{
	HGMovementComp->StartSprint();
}

void AHGCharacter::StopSprint()
{
	HGMovementComp->StopSprint();
}

void AHGCharacter::CrouchButtonPressed()
{
	HGMovementComp->StartCrouch();
}

void AHGCharacter::CrouchButtonReleased()
{
	HGMovementComp->EndCrouch();
}

void AHGCharacter::HeadBob()
{
	// If our velocity is greater than our walk speed (sprint), and if we are NOT falling.
	float PlayerVelocity = GetVelocity().Length();

	if ( (PlayerVelocity >= HGMovementComp->GetWalkSpeed()) && !(GetCharacterMovement()->IsFalling()) )
	{
		APlayerController* MyController = Cast<APlayerController>(GetController());

		/* Weird but it's a self note for myself AND to make it more readable */
		float Value = PlayerVelocity;
		float InRangeA = 0.f;
		float InRangeB = GetCharacterMovement()->MaxWalkSpeed;
		float OutRangeA = 0.f;
		float OutRangeB = 1.f;
		float Scale = UKismetMathLibrary::MapRangeClamped(Value, InRangeA, InRangeB, OutRangeA, OutRangeB);

		// If our velocity is greater than our sprint speed, and if we are NOT falling.
		if (PlayerVelocity >= HGMovementComp->GetSprintSpeed() && !(GetCharacterMovement()->IsFalling()))
		{
			if (MyController)
			{
				MyController->ClientPlayCameraShake(RunCameraShakeClass, Scale);
			}
		}
		else
		{
			if (MyController)
			{
				MyController->ClientPlayCameraShake(WalkCameraShakeClass, Scale);
			}
		}
	}
}

/*
void AHGCharacter::ShortenPlayerCapsule()
{
}
*/

