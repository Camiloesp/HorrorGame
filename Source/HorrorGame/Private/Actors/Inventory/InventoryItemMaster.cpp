// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/InventoryItemMaster.h"

// Sets default values
AInventoryItemMaster::AInventoryItemMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventoryItemMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventoryItemMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInventoryItemMaster::UseItem()
{
}

