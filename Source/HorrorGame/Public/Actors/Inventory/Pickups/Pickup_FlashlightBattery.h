// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Inventory/Pickups/PickupActorMaster.h"
#include "Pickup_FlashlightBattery.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API APickup_FlashlightBattery : public APickupActorMaster
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickup_FlashlightBattery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool PickUp() override;

};
