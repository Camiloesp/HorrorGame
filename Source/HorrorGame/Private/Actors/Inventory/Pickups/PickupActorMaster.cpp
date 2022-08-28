// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/Pickups/PickupActorMaster.h"
#include "Components/SphereComponent.h"
#include "Characters/HGCharacter.h"
#include "Components/InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Widgets/Inventory/PickupPrompt.h"
#include "GameStates/L1GameState.h"

// Sets default values
APickupActorMaster::APickupActorMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(Mesh);
	SphereCollision->SetSphereRadius(500.f);

	PromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Propmt Widget"));
	PromptWidget->SetRelativeScale3D(FVector(0.03f, 0.03f, 0.03f));
	//PromptWidget->SetWidgetClass(); set in BP implementation of this class
	PromptWidget->SetupAttachment(RootComp);
	PromptWidget->SetVisibility(false);
	
	// Variable Declaration
	PickUpDistance = 200.f;
	WidgetDistanceAboveMesh = 25.f;

	PromptWidget->SetRelativeLocation(FVector(0.f, 0.f, WidgetDistanceAboveMesh));
}

// Called when the game starts or when spawned
void APickupActorMaster::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupActorMaster::ShowPromptWidget);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &APickupActorMaster::HidePromptWidget);

	AL1GameState* GameState = Cast<AL1GameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		FObjectiveData CurrentObjective = GameState->FindCurrentObjective();
		QuestID = CurrentObjective.IDName;
	}
}

// Called every frame
void APickupActorMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	FVector Distance;
	Distance.X = Mesh->GetComponentLocation().X;
	Distance.Y = Mesh->GetComponentLocation().Y;
	Distance.Z = Mesh->GetComponentLocation().Z + WidgetDistanceAboveMesh;
	PromptWidget->SetWorldLocation(Distance);

	if (bUpdateActor)
	{
		if (!PickerCharacter) return;

		// if our linetrace detects something in between the PromptWidget and the player camera, do not display the prompt
		FHitResult HitResult;
		const FVector Start = GetActorLocation();
		const FVector End = PickerCharacter->GetFollowCamera()->GetComponentLocation();  
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);

		// Show PromptWidget
		if (!HitResult.bBlockingHit)
		{
			PromptWidget->SetVisibility(true);

			const FVector WidgetStart = PromptWidget->GetComponentLocation();
			const FVector CharacterEnd = PickerCharacter->GetFollowCamera()->GetComponentLocation();
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(WidgetStart, CharacterEnd);
			PromptWidget->SetWorldRotation(NewRotation);
		}
		else
		{
			PromptWidget->SetVisibility(false);
			DisableInput(Cast<APlayerController>(PickerCharacter->GetController()));
		}

		// Length of line trace
		double LinetraceLength = UKismetMathLibrary::VSize(HitResult.TraceStart - HitResult.TraceEnd);
		if (LinetraceLength <= PickUpDistance)
		{
			// Display pick up 'E'
			UPickupPrompt* PickupPrompt = Cast<UPickupPrompt>(PromptWidget->GetUserWidgetObject());
			if (PickupPrompt)
			{
				PickupPrompt->TogglePrompt(true);
			}

			EnableInput(Cast<APlayerController>(PickerCharacter->GetController()));
		}
		else
		{
			// Display pick up Arrow
			UPickupPrompt* PickupPrompt = Cast<UPickupPrompt>(PromptWidget->GetUserWidgetObject());
			if (PickupPrompt)
			{
				PickupPrompt->TogglePrompt(false);
			}
			DisableInput(Cast<APlayerController>(PickerCharacter->GetController()));
		}
	}
}

void APickupActorMaster::ShowPromptWidget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		PickerCharacter = Cast<AHGCharacter>(OtherActor);
		if (PickerCharacter)
		{
			PromptWidget->SetVisibility(true);
			bUpdateActor = true;
		}
	}
}

void APickupActorMaster::HidePromptWidget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		PickerCharacter = Cast<AHGCharacter>(OtherActor);
		if (PickerCharacter)
		{
			PromptWidget->SetVisibility(false);
			bUpdateActor = false;

			DisableInput(Cast<APlayerController>(PickerCharacter->GetController()));
			//PickerCharacter = nullptr;
		}
	}
}

bool APickupActorMaster::PickUp()
{
	bool bSuccess = false;

	if (PickerCharacter)
	{
		UInventoryComponent* PickerInventory = PickerCharacter->GetInventory();
		// if it has an inventory component.
		if (PickerInventory)
		{
			int RemainderItems = 0;
			if (PickerInventory->AddItem(Item, Amount, RemainderItems))
			{
				// If we have some amount left after pickup
				if (RemainderItems > 0)
				{
					//Amount = RemainderItems;
					bSuccess = true;
				}
				else
				{
					//bSuccess = true;
					Destroy();
				}

				bSuccess = true;
			}
			else
			{
				// didnt have the room to pick up actor. Leave actor in the world
				UE_LOG(LogTemp, Warning, TEXT("APickupActorMaster - Item NOT added."));
				bSuccess = false;
			}
		}
	}

	return bSuccess;
}

void APickupActorMaster::Interact()
{
	PickUp();
}
