// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/Pickups/PickupActorMaster.h"
#include "Components/SphereComponent.h"
#include "Characters/HGCharacter.h"
#include "Components/InventoryComponent.h"

// Sets default values
APickupActorMaster::APickupActorMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(100.f);

}

// Called when the game starts or when spawned
void APickupActorMaster::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupActorMaster::Pickup);
}

// Called every frame
void APickupActorMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupActorMaster::Pickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		//Get player picking this up
		AHGCharacter* PickerCharacter = Cast<AHGCharacter>(OtherActor);
		if (PickerCharacter)
		{
			UInventoryComponent* PickerInventory = PickerCharacter->GetInventory();
			if (PickerInventory)
			{
				if ( PickerInventory->AddItem(Item, Amount) )
				{
					Destroy();
				}
				else
				{
					// didnt have the room to pick up actor. Leave actor in the world
				}
			}
		}
	}
}

