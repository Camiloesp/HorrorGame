// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/FlashlightBattery.h"
#include "ItemData.h" // ?


// Sets default values
AFlashlightBattery::AFlashlightBattery()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ItemData.ItemName = FName(TEXT("Flashlight Battery"));
	// ItemData.Icon will be assigned in the BP version of this actor.
	ItemData.Description = FText::FromString(TEXT("Add it to your flashlight to replenish your battery life."));
	ItemData.MaxStackAmount = 8;
	// ItemData.PickupActor will be assigned in the BP version of this actor.
	// ItemData.ExaminationMesh will be assigned in the BP version of this actor.
	ItemData.ExaminationMeshOffset;
}

// Called when the game starts or when spawned
void AFlashlightBattery::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFlashlightBattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlashlightBattery::UseItem()
{
	// Super::UseItem();
	UE_LOG(LogTemp, Warning, TEXT("Using battery"));
}
