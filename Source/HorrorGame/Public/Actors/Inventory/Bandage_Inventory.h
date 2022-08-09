// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "Bandage_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API ABandage_Inventory : public AInventoryItemMaster
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABandage_Inventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
protected:

	/* Amount to replenish health */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float HealthAmountToAdd;

public:

	virtual void UseItem() override;
	
};
