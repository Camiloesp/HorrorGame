// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Locks/Lock.h"
#include "Actors/Locks/LockDial.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/HGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	LockMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LockerMesh"));
	LockMesh->SetupAttachment(RootComp);
	LockMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraView->SetupAttachment(RootComp);
	CameraView->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	CameraView->SetRelativeLocation(FVector(15.f, 0.f, -5.f));
	CameraView->FieldOfView = 45.f;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	BoxCollision->SetupAttachment(RootComp);
	
	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("InteractionWidget"));
	WidgetInteraction->SetupAttachment(RootComp);
	WidgetInteraction->InteractionSource = EWidgetInteractionSource::Mouse; //EWidgetInteractionSource::CenterScreen

	//UWidgetInteractionComponent* WidgetInteraction

	// Initialize variables
	bIsUnlocked = false;
	bPlayerLockView = false;
	BoxPosition = FVector(0.f, 0.f, 0.f);
	BoxExtent = FVector(32.f, 32.f, 32.f);
	CameraBlendTime = 1.f;
	NumberOfDials = 4;

	// Add combination to unlock this lock
	LockCombination.Add(9);
	LockCombination.Add(5);
	LockCombination.Add(1);
	LockCombination.Add(6);
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDials();
}

void ALock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Edit components post variables initialization
	BoxCollision->SetBoxExtent(BoxExtent);
	BoxCollision->SetRelativeLocation(BoxPosition);
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALock::EnablePlayerInput()
{
	APlayerController* PlayerController = Cast<APlayerController>(InteractingPlayer->GetController());
	if (!PlayerController) return;
	EnableInput(PlayerController);
	PlayerController->bShowMouseCursor = true;


	// Bind functions if they haven't been binded. (Avoids check break in editor)
	bool ReturnButtonFunctionBinded = InteractingPlayer->OnReturnButtonPressed.Contains(this, FName(TEXT("ExitLockView")));
	bool InteractButtonFunctionBinded = InteractingPlayer->OnInteractButtonPressed.Contains(this, FName(TEXT("ExitLockView")));
	if (!ReturnButtonFunctionBinded)
	{
		InteractingPlayer->OnReturnButtonPressed.AddDynamic(this, &ALock::ExitLockView);
	}
	if (!InteractButtonFunctionBinded)
	{
		InteractingPlayer->OnInteractButtonPressed.AddDynamic(this, &ALock::ExitLockView);
	}

	//Displays UI arrows
	ToggleLockDialsArrowsUI();
}

void ALock::ToggleLockDialsArrowsUI()
{
	for (ALockDial* CurrentDial : Dials)
	{
		bool bNewVisibility = !CurrentDial->GetUpArrow()->IsVisible();
		CurrentDial->GetUpArrow()->SetVisibility(bNewVisibility);
		CurrentDial->GetDownArrow()->SetVisibility(bNewVisibility);
	}
}

void ALock::DestroyLock()
{
	for (ALockDial* CurrentDial : Dials)
	{
		CurrentDial->Destroy();
	}
	Destroy();
}

void ALock::SpawnDials()
{
	//Spawns dials
	for (int i = 0; i < NumberOfDials; i++) // <=3
	{
		if (LockDialClass)
		{
			// Create string name from socket in the LockMesh
			FString SocketString = TEXT("DialBone");
			SocketString.Append(FString::FromInt(i + 1));
			SocketString.Append(TEXT("Socket"));
			FName SocketNameToAttach = FName(*SocketString);

			// Spawn location and rotation
			FVector SpawnLocation = FVector::ZeroVector;
			FRotator SpawnRotation = FRotator::ZeroRotator;
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn dial and add it to array
			ALockDial* SpawnedDial = GetWorld()->SpawnActor<ALockDial>(LockDialClass, SpawnLocation, SpawnRotation, SpawnParams);
			SpawnedDial->SetActorRotation(GetActorRotation()); //test
			SpawnedDial->SetIndex(i);
			SpawnedDial->SetLockRef(this);
			Dials.Add(SpawnedDial);

			// Attach spawned actors to this actor mesh
			EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule RotationRule = EAttachmentRule::KeepWorld; //KeepWorld
			EAttachmentRule ScaleRule = EAttachmentRule::KeepWorld;
			FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, true);
			Dials[i]->AttachToComponent(LockMesh, AttachmentRules, SocketNameToAttach);
		}
	}
}

bool ALock::CheckCode()
{
	// if all of our 4 combinations are equal then bLocalSuccess is always true
	bool bLocalSuccess = false;

	for (int i=0; i<Dials.Num(); i++)
	{
		ALockDial* Dial = Dials[i];

		int CurrentDialNumber = Dial->GetNumber();
		if (LockCombination[i] == CurrentDialNumber)
		{
			bLocalSuccess = true;
		}
		else
		{
			bLocalSuccess = false;
			break; // stop loop
		}
	}

	return bLocalSuccess;
}

void ALock::ExitLockView()
{
	if (!InteractingPlayer) return;

	// Look through the camera to mess with dials
	APlayerController* PlayerController = Cast<APlayerController>(InteractingPlayer->GetController());
	if (!PlayerController) return;
	DisableInput(PlayerController);
	PlayerController->bShowMouseCursor = false;


	InteractingPlayer->OnReturnButtonPressed.RemoveDynamic(this, &ALock::ExitLockView);
	InteractingPlayer->OnInteractButtonPressed.RemoveDynamic(this, &ALock::ExitLockView);
	InteractingPlayer->OnLeftMouseButtonPressed.RemoveDynamic(this, &ALock::LeftMouseButtonPressed);
	InteractingPlayer->OnLeftMouseButtonReleased.RemoveDynamic(this, &ALock::LeftMouseButtonReleased);

	// Slowly blend back to our player camera.
	PlayerController->SetViewTargetWithBlend(InteractingPlayer, CameraBlendTime);
	bPlayerLockView = false;

	// enable look input
	PlayerController->ResetIgnoreLookInput();

	// enable inventory
	InteractingPlayer->SetCanOpenInventory(true);

	// Enable movement
	InteractingPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//Hides UI arrows
	ToggleLockDialsArrowsUI();

	// no longer interacting.
	InteractingPlayer = nullptr;
}

void ALock::LeftMouseButtonPressed()
{
	WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void ALock::LeftMouseButtonReleased()
{
	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void ALock::CheckEveryTurn()
{
	if (!UnlockAnimation) return;

	bool bCorrectCombination = CheckCode();
	if (bCorrectCombination)
	{
		LockMesh->PlayAnimation(UnlockAnimation, false);

		float Delay = UnlockAnimation->GetMaxCurrentTime();
		
		FTimerHandle ExitHandle;
		GetWorld()->GetTimerManager().SetTimer(ExitHandle, this, &ALock::ExitLockView, Delay, false);

		// Destroy this lock and its dials
		FTimerHandle DestroyHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroyHandle, this, &ALock::DestroyLock, Delay+1.f, false);
	}
}

void ALock::Interact()
{
	if (bPlayerLockView) return; // Player in camera view, don't execute Interact functionality, let delegate handle the input instead.
	if (!InteractingPlayer) return;

	// Look through the camera to mess with dials
	APlayerController* PlayerController = Cast<APlayerController>(InteractingPlayer->GetController());
	if (!PlayerController) return;

	// Slowly blend to this actor camera.
	PlayerController->SetViewTargetWithBlend(this, CameraBlendTime);
	bPlayerLockView = true;

	// Disable movement
	InteractingPlayer->GetCharacterMovement()->DisableMovement();

	//Disable look input
	PlayerController->SetIgnoreLookInput(false);

	// Cannot open inventory
	InteractingPlayer->SetCanOpenInventory(false);

	//Bind left mouse button pressed/released delegate
	InteractingPlayer->OnLeftMouseButtonPressed.AddDynamic(this, &ALock::LeftMouseButtonPressed);
	InteractingPlayer->OnLeftMouseButtonReleased.AddDynamic(this, &ALock::LeftMouseButtonReleased);

	//Enable input
	FTimerHandle EnableInputTimer;
	GetWorld()->GetTimerManager().SetTimer(EnableInputTimer, this, &ALock::EnablePlayerInput, CameraBlendTime, false);
}
