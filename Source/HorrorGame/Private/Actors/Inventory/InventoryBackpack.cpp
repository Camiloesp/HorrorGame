// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/InventoryBackpack.h"
#include "Components/SphereComponent.h"
#include "Characters/HGCharacter.h"
#include "Components/InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Widgets/Inventory/PickupPrompt.h"

// Sets default values
AInventoryBackpack::AInventoryBackpack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	BackpackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	BackpackMesh->SetupAttachment(RootComp);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComp);
	SphereCollision->SetSphereRadius(500.f);

	PromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PromptWidget->SetupAttachment(RootComp);
	PromptWidget->SetVisibility(false);

	AmountOfSlots = 2;
	WidgetDistanceAboveMesh = 30.f;
	PickUpDistance = 200.f;
}

// Called when the game starts or when spawned
void AInventoryBackpack::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AInventoryBackpack::ShowPromptWidget);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AInventoryBackpack::HidePromptWidget);
}

// Called every frame
void AInventoryBackpack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set distance
	FVector Distance;
	Distance.X = BackpackMesh->GetComponentLocation().X;
	Distance.Y = BackpackMesh->GetComponentLocation().Y;
	Distance.Z = BackpackMesh->GetComponentLocation().Z + WidgetDistanceAboveMesh;
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

void AInventoryBackpack::PickUp()
{
	if (!PickerCharacter) return;

	// Adds more inventory slots to the player InventoryComponent and THEN gets destroyed.
	PickerCharacter->GetInventory()->AddMoreSlots(AmountOfSlots);
	Destroy();
}

void AInventoryBackpack::Interact()
{
	PickUp();
}

void AInventoryBackpack::ShowPromptWidget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AInventoryBackpack::HidePromptWidget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

