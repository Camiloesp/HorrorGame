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
	ItemData.MaxStackAmount = 8;
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