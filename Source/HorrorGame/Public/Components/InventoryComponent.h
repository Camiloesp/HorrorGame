// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItems.h"
#include "InventoryComponent.generated.h"

class AInventoryItemMaster;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


protected:

	/* 
	* Checks if there is an empty slot in our inventory.
	* @param EmptySlotIndex Out-parameter of the empty slot index if found.
	* @return True if found an empty slot, false otherwise.
	*/
	bool CheckForEmptySlot(int& EmptySlotIndex);

public:

	/*
	* Adds item to inventory
	* @param Item Item being added
	* @param Amount Stack amount
	* @return True if item was added, false otherwise
	*/
	bool AddItem(AInventoryItemMaster* Item, int Amount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInventoryItems> InventorySlots;
};
