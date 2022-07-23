// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "DoorKey_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API ADoorKey_Inventory : public AInventoryItemMaster
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADoorKey_Inventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
