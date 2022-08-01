// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItems.h"
#include "InventoryComponent.generated.h"

class AInventoryItemMaster;
class UInventoryMenu;
class AHGCharacter;

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


	/*
	* Updates inventory widget at a specific slot.
	* @param IndexSlot Index for the inventory widget to be updated.
	*/
	void UpdateInventorySlot(int IndexSlot);

	UInventoryMenu* InventoryMenuRef;

	AHGCharacter* PlayerRef;
protected:

	/* 
	* Checks if there is an empty slot in our inventory.
	* @param EmptySlotIndex Out-parameter of the empty slot index if found.
	* @return True if found an empty slot, false otherwise.
	*/
	bool CheckForEmptySlot(int& EmptySlotIndex);

	/*
	* Check for a slot that has room for the same item, if it's allowed to stack.
	* @param NewItem Item class to look for in the inventory to see if we can stack it.
	* @param OutIndex Returns an index for the InventorySlots where we can stack NewItem
	* @return True if found an inventory slot where we can stack our NewItem
	*/
	bool CheckForFreeSlot(TSubclassOf<AInventoryItemMaster>& NewItem, int& OutIndex);

public:

	/*
	* Uses the item at a index in our inventory.
	* @param SlotIndex Index for the item in the Slot Array we want to use.
	*/
	void UseItem(int SlotIndex);

	/*
	* Removes the item at a index in our inventory.
	* @param SlotIndex Index for the item in the Slot Array we want to remove.
	*/
	void RemoveItem(int SlotIndex);

	/*
	* Drops an intem from our inventory into the world.
	* @param SlotIndex Index for the item in the Slot array we want to drop.
	*/
	void DropItem(int SlotIndex);

	/*
	* Adds item to inventory
	* @param NewItem Item being added
	* @param Amount Stack amount
	* @param Remainder Return output in case we can pickup some of the items, but not all of them.
	* @return True if item was added, false otherwise
	*/ // AInventoryItemMaster* Item
	bool AddItem(TSubclassOf<AInventoryItemMaster> NewItem, int Amount, int& Remainder);

	/*
	* Get item data based on an inventory index slot
	* @param Index Inventory index slot to look for the item data.
	* @param OutItem Returns the Item stored at Index.
	* @param OutAmount Returns the amount stored at Index.
	*/
	void GetItemDataAtIndex(int Index, TSubclassOf<AInventoryItemMaster>& OutItem, int& OutAmount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInventoryItems> InventorySlots;

	//UInventoryMenu* InventoryMenuRef;
	FORCEINLINE UInventoryMenu* GetInventoryMenuRef() const { return InventoryMenuRef; }

	FORCEINLINE void SetInventoryMenuRef(UInventoryMenu* NewInventoryMenuRef) { InventoryMenuRef = NewInventoryMenuRef; }
};
