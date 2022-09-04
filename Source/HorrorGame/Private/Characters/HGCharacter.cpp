// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Interaction.h"
#include "Interfaces/GrabInterface.h"
#include "Widgets/MainHUD.h"
#include "Widgets/Inventory/InventoryMenu.h"
#include "Components/Movement.h"
#include "Kismet/KismetMathLibrary.h"
#include "Controllers/HGPlayerController.h"
#include "Components/InventoryComponent.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Actors/Inventory/Pickups/PickupActorMaster.h"
#include "Widgets/Inventory/ExaminationWidget.h"
#include "Components/FlashlightComponent.h"
#include "Components/HealthComponent.h"
#include "Widgets/ProgressBars.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Widgets/CameraWidget.h"

#include "GameStates/L1GameState.h"

#include "AI/Classic/AI_Basic.h"




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
	Flashlight->SetVisibility(false);
	//Flashlight transform to 0.f

	HGMovementComponent = CreateDefaultSubobject<UMovement>(TEXT("Movement Comp"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	FlashlightComponent = CreateDefaultSubobject<UFlashlightComponent>(TEXT("Flashlight Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->JumpZVelocity = 400.f;

	// Variables Initialization
	TurnRate = 0.4;
	DistanceToInteract = 350.f;
	CrouchSoundStepsSpeed = 1.f;
	WalkingSoundStepsSpeed = 0.5f;
	SprintSoundStepsSpeed = 0.3f;
	MinFOV = 30.f;
	MaxFOV = 0.f; //Set again in Initialize()
	bIsInventoryOpen = false;
	bIsHiding = false;
	bCanOpenInventory = true;
	bReadingNote = false;
	bCameraWidgetOpen = false;
}

// Called when the game starts or when spawned
void AHGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();


	//PlayFootstep
	GetWorld()->GetTimerManager().SetTimer(WalkingSoundTimer, this, &AHGCharacter::PlayFootstep, WalkingSoundStepsSpeed,true);
}

// Called every frame
void AHGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateDOF();
}

// Called to bind functionality to input
void AHGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComp = PlayerInputComponent;

	/* BIND AXIS */
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AHGCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AHGCharacter::LookRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHGCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHGCharacter::MoveRight);

	/* BIND ACTIONS */
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AHGCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Action"), EInputEvent::IE_Pressed, this, &AHGCharacter::ActionButtonPressed);

	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), EInputEvent::IE_Pressed, this, &AHGCharacter::LeftMouseButtonPressed);
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), EInputEvent::IE_Released, this, &AHGCharacter::LeftMouseButtonReleased);

	PlayerInputComponent->BindAction(TEXT("FlashlightToggle"), EInputEvent::IE_Released, this, &AHGCharacter::ToggleFlashlight);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AHGCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AHGCharacter::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AHGCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &AHGCharacter::CrouchButtonReleased);
	
	PlayerInputComponent->BindAction(TEXT("InventoryToggle"), EInputEvent::IE_Pressed, this, &AHGCharacter::InventoryButtonPressed);

	PlayerInputComponent->BindAction(TEXT("Return"), EInputEvent::IE_Pressed, this, &AHGCharacter::ReturnButtonPressed);

	PlayerInputComponent->BindAction(TEXT("Camera"), EInputEvent::IE_Pressed, this, &AHGCharacter::CameraButtonPressed);

	PlayerInputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AHGCharacter::ZoomInPressed);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AHGCharacter::ZoomOutPressed);

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
	AActor* Actionee = LineTrace(DistanceToInteract);
	// Interacts with the actor, if is a child of the IInteraction.
	IInteraction* InteractingActor = Cast<IInteraction>(Actionee);
	if (InteractingActor) // Interact
	{
		// Sets reference to the player interacting with the interactable and then executing Interaction logic.
		InteractingActor->InteractingPlayer = this;
		InteractingActor->Interact();

		OnInteractButtonPressed.Broadcast();
	}
	else //Lean to the right
	{
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

	OnLeftMouseButtonPressed.Broadcast();
}

void AHGCharacter::LeftMouseButtonReleased()
{
	if (GrabbedActor)
	{
		GrabbedActor->ReleaseObject();
		GrabbedActor->GrabberPlayer = nullptr;
		GrabbedActor = nullptr;
	}

	OnLeftMouseButtonReleased.Broadcast();
}

void AHGCharacter::ReturnButtonPressed()
{
	OnReturnButtonPressed.Broadcast();
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
	// Get reference to our controller
	ControllerRef = Cast<AHGPlayerController>(GetController());

	//FlashlightComponent->SetFlashlightLight(Flashlight);
	FlashlightComponent->Initialize(this);

	//Create HUD widget, and add it to viewport.
	if (PlayerHUDClass)
	{
		MainHUDRef = CreateWidget<UMainHUD>(GetWorld(), PlayerHUDClass);
		if (MainHUDRef)
		{
			MainHUDRef->AddToViewport();
			MainHUDRef->SetPlayerOwner(this);
			MainHUDRef->GetProgressBars()->SetPlayerOwner(this);
		}
	}

	//Create inventory widget, and add it to viewport.
	if (PlayerInventoryClass)
	{
		InventoryMenuRef = CreateWidget<UInventoryMenu>(GetWorld(), PlayerInventoryClass);
		if (InventoryMenuRef)
		{
			InventoryMenuRef->SetVisibility(ESlateVisibility::Collapsed);
			InventoryMenuRef->AddToViewport();


			if (InventoryComponent)
			{
				InventoryComponent->SetInventoryMenuRef(InventoryMenuRef);
			}

			// Create Camera widget
			CameraWidget = CreateWidget<UCameraWidget>(GetWorld(), CameraWidgetClass);
			if (CameraWidget)
			{
				// Do not add to viewport. only when we press the R key
			}
		}
	}

	// Tell the GameState we are ready to update our first objective in our HUD.
	AL1GameState* GameState = Cast<AL1GameState>(GetWorld()->GetGameState());
	if (GameState && MainHUDRef)
	{
		GameState->Initialize(MainHUDRef);
	}

	MaxFOV = FollowCamera->FieldOfView;


	// Initialize our custom movement component
	HGMovementComponent->Initialize(this);


	// Enable input. (When AI catches us we disable it and when we restart the level we cannot move)
	if (!ControllerRef) return;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	ControllerRef->ResetIgnoreLookInput();
	ControllerRef->bShowMouseCursor = false;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(ControllerRef);
}

void AHGCharacter::ToggleFlashlight()
{
	FlashlightComponent->ToggleFlashlight();
}

void AHGCharacter::Sprint()
{
	HGMovementComponent->StartSprint();
	
	// Increment timer call
	GetWorld()->GetTimerManager().ClearTimer(WalkingSoundTimer);
	GetWorld()->GetTimerManager().SetTimer(WalkingSoundTimer, this, &AHGCharacter::PlayFootstep, SprintSoundStepsSpeed, true);
}

void AHGCharacter::StopSprint()
{
	HGMovementComponent->StopSprint();

	// Decrement timer call
	GetWorld()->GetTimerManager().ClearTimer(WalkingSoundTimer);
	GetWorld()->GetTimerManager().SetTimer(WalkingSoundTimer, this, &AHGCharacter::PlayFootstep, WalkingSoundStepsSpeed, true);
}

void AHGCharacter::CrouchButtonPressed()
{
	HGMovementComponent->StartCrouch();

	//CrouchSoundStepsSpeed
	// Increment timer call
	GetWorld()->GetTimerManager().ClearTimer(WalkingSoundTimer);
	GetWorld()->GetTimerManager().SetTimer(WalkingSoundTimer, this, &AHGCharacter::PlayFootstep, CrouchSoundStepsSpeed, true);
}

void AHGCharacter::CrouchButtonReleased()
{
	HGMovementComponent->EndCrouch();

	// Decrement timer call
	GetWorld()->GetTimerManager().ClearTimer(WalkingSoundTimer);
	GetWorld()->GetTimerManager().SetTimer(WalkingSoundTimer, this, &AHGCharacter::PlayFootstep, WalkingSoundStepsSpeed, true);
}

void AHGCharacter::StartJump()
{
	Jump();
	OnJumpButtonPressed.Broadcast();
}

void AHGCharacter::HeadBob()
{
	// If our velocity is greater than our walk speed (sprint), and if we are NOT falling.
	float PlayerVelocity = GetVelocity().Length();

	if ( (PlayerVelocity >= HGMovementComponent->GetWalkSpeed()) && !(GetCharacterMovement()->IsFalling()) )
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
		if (PlayerVelocity >= HGMovementComponent->GetSprintSpeed() && !(GetCharacterMovement()->IsFalling()))
		{
			if (MyController)
			{
				MyController->ClientStartCameraShake(RunCameraShakeClass, Scale);
			}
		}
		else
		{
			if (MyController)
			{
				MyController->ClientStartCameraShake(WalkCameraShakeClass, Scale);
			}
		}
	}
}

void AHGCharacter::InventoryButtonPressed()
{
	if (!bReadingNote)
	{
		ToggleInventory();
	}
	OnInventoryButtonPressed.Broadcast();
}

void AHGCharacter::CameraButtonPressed()
{
	if (!CameraWidget) return;
	
	if (bCameraWidgetOpen)
	{
		CameraWidget->RemoveFromParent();
		MainHUDRef->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FollowCamera->SetFieldOfView(MaxFOV);
		OnCameraButtonPressed.Broadcast(false);
	}
	else
	{
		CameraWidget->AddToViewport();
		MainHUDRef->SetVisibility(ESlateVisibility::Hidden);
		OnCameraButtonPressed.Broadcast(true);
	}

	bCameraWidgetOpen = !bCameraWidgetOpen;
}

void AHGCharacter::ZoomInPressed()
{
	if (!bCameraWidgetOpen) return;

	float NewFOV = FollowCamera->FieldOfView + (-5.f);
	float FinalFOV = UKismetMathLibrary::FClamp(NewFOV, MinFOV, MaxFOV);

	FollowCamera->SetFieldOfView(FinalFOV);
}

void AHGCharacter::ZoomOutPressed()
{
	if (!bCameraWidgetOpen) return;

	float NewFOV = FollowCamera->FieldOfView + (5.f);
	float FinalFOV = UKismetMathLibrary::FClamp(NewFOV, MinFOV, MaxFOV);

	FollowCamera->SetFieldOfView(FinalFOV);
}

void AHGCharacter::PlayFootstep()
{
	float CurrentVelocityLength = GetCharacterMovement()->Velocity.Length();
	if(CurrentVelocityLength > 0.f && !GetCharacterMovement()->IsFalling())
	{
		// linetrace to see physical material under player
		FHitResult OutHitResult;
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + FVector(0.f, 0.f, -100.f); // below us
		FCollisionQueryParams QueryParams; //test
		QueryParams.bReturnPhysicalMaterial = true;//test

		GetWorld()->LineTraceSingleByChannel(OutHitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, QueryParams);
		if (OutHitResult.bBlockingHit && OutHitResult.PhysMaterial.Get())
		{
			// Select material to play
			//UPhysicalMaterial* PhysicalMaterialHit = UPhysicalMaterial::DetermineSurfaceType(OutHitResult.PhysMaterial.Get()); //OutHitResult.PhysMaterial.Get();
			//if (PhysicalMaterialHit)
			{
				USoundBase* FootstepSoundToPlay;
				// EPhysicalSurface SurfaceType = PhysicalMaterialHit->SurfaceType;
				//EPhysicalSurface SurfaceType = OutHitResult.PhysMaterial->SurfaceType;
				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(OutHitResult.PhysMaterial.Get());

				switch (SurfaceType)
				{
					case SurfaceType1: // tile
						FootstepSoundToPlay = TileFootstep;
						break;
					case SurfaceType2: // grass
						FootstepSoundToPlay = GrassFootstep;
						break;
					default:
						FootstepSoundToPlay = TileFootstep;
				}
				
				float NewVolume = 0.f;
				UKismetMathLibrary::MapRangeUnclamped(GetVelocity().Length(), CrouchSoundStepsSpeed, WalkingSoundStepsSpeed, 0.5f, 1.f);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSoundToPlay, OutHitResult.ImpactPoint);
			}
		}
	}
}
void AHGCharacter::UpdateDOF()
{
	float LocalDOF = 0.f;

	FHitResult HitResultDOF;
	FVector TraceStart = FollowCamera->GetComponentLocation();
	FVector TraceEnd = (FollowCamera->GetForwardVector() * 300.f) + TraceStart;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResultDOF, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (bHit)
	{
		LocalDOF = (HitResultDOF.Location - TraceStart).Length();
	}
	
	FPostProcessSettings NewPostProcessSettings;
	NewPostProcessSettings.DepthOfFieldFocalDistance = LocalDOF;
	NewPostProcessSettings.DepthOfFieldDepthBlurRadius = (LocalDOF == 0.f) ? 0.f : 500.f;

	FollowCamera->PostProcessSettings = NewPostProcessSettings;
}

void AHGCharacter::CaughtByEnemy(APawn* Enemy)
{
	/*------------------------------------------*/
	// Clasic health reduction approach
	//float DamageTaken = 30.f;
	//HealthComponent->RemoveHealth(DamageTaken);
	/*------------------------------------------*/

	/*------------------------------------------*/
	/* Grab player approach */
	EnemyAttacking = Enemy;
	// Disable input
	GetCharacterMovement()->DisableMovement();
	APlayerController* OurController = Cast<APlayerController>(GetController());
	if (!OurController) return;
	DisableInput(OurController);
	OurController->SetIgnoreLookInput(true);
	
	// Delat turning player
	GetWorld()->GetTimerManager().SetTimer(RotateToEnemyHandle, this, &AHGCharacter::RotateToEnemy, 0.01f, true);
}

void AHGCharacter::RotateToEnemy()
{
	APlayerController* OurController = Cast<APlayerController>(GetController());
	if (!OurController || !EnemyAttacking) return;

	// Calculate new Controller rotation.
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = EnemyAttacking->GetActorLocation();
	FRotator NewControllerRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	// Calculate New controller rotation interpolated.
	FRotator ControlRotation = OurController->GetControlRotation();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FRotator NewControllerRotationInterp = UKismetMathLibrary::RInterpTo(ControlRotation, NewControllerRotation, DeltaTime, 10.f);
	OurController->SetControlRotation(NewControllerRotationInterp);

	// if we are in our target rotation, stop timer!
	if (UKismetMathLibrary::EqualEqual_RotatorRotator(ControlRotation, NewControllerRotation, 1.f))
	{
		GetWorld()->GetTimerManager().ClearTimer(RotateToEnemyHandle);

		//Call attack animation from AI.
		AAI_Basic* EnemyAttacker = Cast<AAI_Basic>(EnemyAttacking);
		if (!EnemyAttacker) return;

		// DoOnce here?
		EnemyAttacker->KillPlayer();
	}
}

void AHGCharacter::ToggleInventory()
{
	if (!ControllerRef) return;
	if (!bCanOpenInventory) return;

	if (bIsInventoryOpen)
	{
		bIsInventoryOpen = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); //this
		ControllerRef->ResetIgnoreLookInput();//this
		ControllerRef->bShowMouseCursor = false;//this
		if (InventoryMenuRef)
		{
			InventoryMenuRef->SetVisibility(ESlateVisibility::Collapsed);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(ControllerRef);//this
		}
	}
	else
	{
		bIsInventoryOpen = true;
		GetCharacterMovement()->DisableMovement();//this
		ControllerRef->SetIgnoreLookInput(true); //this
		ControllerRef->bShowMouseCursor = true;//this

		// remove examine widget if open
		if (ExaminationWidget && ExaminationWidget->IsInViewport())
		{
			ExaminationWidget->RemoveFromParent();
		}

		if (InventoryMenuRef)
		{
			InventoryMenuRef->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(ControllerRef, InventoryMenuRef);//this
		}
	}
}

/*
void AHGCharacter::ShortenPlayerCapsule()
{
}
*/

