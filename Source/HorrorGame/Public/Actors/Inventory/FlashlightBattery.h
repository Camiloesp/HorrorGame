// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "FlashlightBattery.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API AFlashlightBattery : public AInventoryItemMaster
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AFlashlightBattery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseItem() override;

};
