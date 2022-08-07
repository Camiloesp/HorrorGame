// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Inventory/Bandage_Inventory.h"

ABandage_Inventory::ABandage_Inventory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemData.ItemName = FName(TEXT("Bandage"));
	// ItemData.Icon will be assigned in the BP version of this actor.
	ItemData.Description = FText::FromString(TEXT("Replenishes all of your health"));
	ItemData.MaxStackAmount = 12;
	// ItemData.PickupActor will be assigned in the BP version of this actor.
	// ItemData.ExaminationMesh will be assigned in the BP version of this actor.
	ItemData.ExaminationMeshOffset = 10.f;
	ItemData.ExaminationMeshRotation = FRotator(0.f, 90.f, 0.f);
}

void ABandage_Inventory::BeginPlay()
{
	Super::BeginPlay();
}

void ABandage_Inventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABandage_Inventory::UseItem()
{

}
