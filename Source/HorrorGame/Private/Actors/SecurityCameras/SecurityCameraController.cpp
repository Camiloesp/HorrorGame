// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SecurityCameras/SecurityCameraController.h"
#include "Characters/HGCharacter.h"
#include "Widgets/MainHUD.h"
#include "Kismet/GameplayStatics.h"

#include "Components/Movement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/SecurityCameras/SecurityCamera.h"

// Sets default values
ASecurityCameraController::ASecurityCameraController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneComp);

	ComputerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ComputerMesh->SetupAttachment(RootComponent);


	bIsViewingCameras = false;
	CurrentCameraIndex = 0;
}

// Called when the game starts or when spawned
void ASecurityCameraController::BeginPlay()
{
	Super::BeginPlay();
	
	// Populate SecurityCameras array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SecurityCameraClass, SecurityCameras);
}

// Called every frame
void ASecurityCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASecurityCameraController::LeaveCameraController()
{
	if (!InteractingPlayerController || !InteractingPlayer) return;

	bIsViewingCameras = false;

	//DisableInput(InteractingPlayerController);
	//InteractingPlayerController->ResetIgnoreLookInput();
	//InteractingPlayer->GetMainHUDRef()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//InteractingPlayer->EnableInput(InteractingPlayerController); // ?

	// Disable movement
	InteractingPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	//Disable look input
	InteractingPlayerController->ResetIgnoreLookInput();
	// Cannot open inventory
	InteractingPlayer->SetCanOpenInventory(true);
	//Hide HUD
	InteractingPlayer->GetMainHUDRef()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// Removing bindings
	InteractingPlayer->OnJumpButtonPressed.RemoveDynamic(this, &ASecurityCameraController::ViewThroughCamera);
	//InteractingPlayer->OnInteractButtonPressed.RemoveDynamic(this, &ASecurityCameraController::LeaveCameraController);
	InteractingPlayer->OnInventoryButtonPressed.RemoveDynamic(this, &ASecurityCameraController::LeaveCameraController);

	InteractingPlayerController->SetViewTargetWithBlend(InteractingPlayer);

	CurrentCameraIndex = 0;
}

void ASecurityCameraController::ViewThroughCamera()
{
	if (!InteractingPlayerController) return;

	if (CurrentCameraIndex >= SecurityCameras.Num())
	{
		CurrentCameraIndex = 0;
	}

	InteractingPlayerController->SetViewTargetWithBlend(SecurityCameras[CurrentCameraIndex]);	
	
	CurrentCameraIndex++;
}

void ASecurityCameraController::Interact()
{
	if (!InteractingPlayer) return;

	if (bIsViewingCameras)
	{
		LeaveCameraController(); //never called here, usually called with bidnings
	}
	else
	{
		bIsViewingCameras = true;
		InteractingPlayerController = Cast<APlayerController>(InteractingPlayer->GetController());
		if (InteractingPlayerController)
		{
			// Change camera view binding.
			InteractingPlayer->OnJumpButtonPressed.AddDynamic(this, &ASecurityCameraController::ViewThroughCamera);
			// Exit SecurityCameraController binding
			//InteractingPlayer->OnInteractButtonPressed.AddDynamic(this, &ASecurityCameraController::LeaveCameraController);
			InteractingPlayer->OnInventoryButtonPressed.AddDynamic(this, &ASecurityCameraController::LeaveCameraController);

			//InteractingPlayer->DisableInput(InteractingPlayerController); // ?
			//InteractingPlayerController->SetIgnoreLookInput(true);
			//InteractingPlayer->GetMainHUDRef()->SetVisibility(ESlateVisibility::Hidden);
			//EnableInput(InteractingPlayerController); //Enable input here

			// Disable movement
			InteractingPlayer->GetCharacterMovement()->DisableMovement();
			//Disable look input
			InteractingPlayerController->SetIgnoreLookInput(true);
			// Cannot open inventory
			InteractingPlayer->SetCanOpenInventory(false);
			//Hide HUD
			InteractingPlayer->GetMainHUDRef()->SetVisibility(ESlateVisibility::Hidden);

			ViewThroughCamera();
		}
	}
}

