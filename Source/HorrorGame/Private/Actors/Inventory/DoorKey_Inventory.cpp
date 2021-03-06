// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/DoorKey_Inventory.h"

ADoorKey_Inventory::ADoorKey_Inventory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemData.ItemName = FName(TEXT("Door Key"));
	// ItemData.Icon will be assigned in the BP version of this actor.
	ItemData.MaxStackAmount = 1;
}

void ADoorKey_Inventory::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorKey_Inventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
