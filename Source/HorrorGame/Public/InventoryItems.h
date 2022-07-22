// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "InventoryItems.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItems
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AInventoryItemMaster> Item;
	//class AInventoryItemMaster* Item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Amount;
};
