// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/DoorKey_Inventory.h"

ADoorKey_Inventory::ADoorKey_Inventory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemData.ItemName = FName(TEXT("Door Key"));
	// ItemData.Icon will be assigned in the BP version of this actor.
	ItemData.Description = FText::FromString(TEXT("Use this to open a locked door."));
	ItemData.MaxStackAmount = 1;
	ItemData.bCanBeUsed = false;
	ItemData.ExaminationMeshRotation = FRotator(-90.f, 0.f, 0.f);
}

void ADoorKey_Inventory::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorKey_Inventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorKey_Inventory::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Using Key"));
}
