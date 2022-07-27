// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
//#include "Kismet/GameplayStatics.h"
#include "Controllers/HGPlayerController.h"
#include "GameFramework/Character.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "Characters/HGCharacter.h"
#include "Widgets/Inventory/InventoryMenu.h"
#include "Widgets/Inventory/InventoryGrid.h"
#include "Widgets/Inventory/InventorySlot.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	/* IF the owner is a player */
	ACharacter* CharOwner = Cast<ACharacter>(GetOwner());
	if (CharOwner)
	{
		AHGPlayerController* OwnerController = Cast<AHGPlayerController>(CharOwner->GetController());
		if (OwnerController)
		{
			InventorySlots.SetNum(OwnerController->GetInventorySlots());
		}
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::UpdateInventorySlot(int IndexSlot)
{
	AHGCharacter* PlayerOwner = Cast<AHGCharacter>(GetOwner());
	if (PlayerOwner)
	{
		// Get inventory slots
		TArray<UInventorySlot*> InventoryGridSlots = PlayerOwner->GetInventoryMenu()->GetInventoryGrid()->GetSlotsArray();

		// Get inventory slot at IndexSlot and update it.
		UInventorySlot* InventorySlotToUpdate = InventoryGridSlots[IndexSlot];
		if (InventorySlotToUpdate)
		{
			InventorySlotToUpdate->UpdateSlot();
		}
	}
}

bool UInventoryComponent::AddItem(TSubclassOf<AInventoryItemMaster> NewItem, int Amount, int& Remainder)
{
	// Set local variables
	AInventoryItemMaster* LocalItem = Cast<AInventoryItemMaster>(NewItem.GetDefaultObject());
	int LocalAmount = Amount;
	int LocalMaxStackAmount = LocalItem->ItemData.MaxStackAmount;

	// Allow inventory item stacking if MaxStackAmout is > 1
	if (LocalMaxStackAmount > 1)
	{
		// CheckForFreeSlot will be true if it found the same item in the inventory, with the Amount being less than MaxStackAmount. (TO BE ABLE TO STACK.)
		int NewIndex = 0;
		if (CheckForFreeSlot(NewItem, NewIndex))
		{
			int CurrentAmountInSlot = 0;
			GetItemDataAtIndex(NewIndex, NewItem, CurrentAmountInSlot);

			if ((CurrentAmountInSlot + LocalAmount) > LocalMaxStackAmount)
			{
				LocalAmount = (CurrentAmountInSlot + LocalAmount) - LocalMaxStackAmount;

				// Add pickup to inventory
				FInventoryItems ItemToAdd;
				ItemToAdd.Item = NewItem; //ItemToAdd.Item = LocalItem->GetClass();
				ItemToAdd.Amount = LocalMaxStackAmount;
				InventorySlots[NewIndex] = ItemToAdd; //InventorySlots.Insert(ItemToAdd, NewIndex);

				// update inventory widget amount text
				UpdateInventorySlot(NewIndex);

				// Add or try adding the leftover Amount in a new slot
				int NewRemainder = 0;
				AddItem(NewItem, LocalAmount, NewRemainder);

				Remainder = NewRemainder;
				return true;
			}
			else
			{
				// Add pickup to inventory
				FInventoryItems ItemToAdd;
				ItemToAdd.Item = NewItem;//ItemToAdd.Item = LocalItem->GetClass();
				ItemToAdd.Amount = CurrentAmountInSlot + LocalAmount;
				//InventorySlots.Insert(ItemToAdd, NewIndex);
				InventorySlots[NewIndex] = ItemToAdd;

				UpdateInventorySlot(NewIndex);

				Remainder = 0;
				return true;
			}
		}
		else // Not able to stack
		{
			// check for an empty slot
			if (CheckForEmptySlot(NewIndex))
			{
				// if the Amount of the item being picked up is greater than the MaxStackAmount
				if (LocalAmount > LocalMaxStackAmount)
				{
					// Add pickup to inventory
					FInventoryItems ItemToAdd;
					ItemToAdd.Item = NewItem;
					ItemToAdd.Amount = LocalMaxStackAmount;
					InventorySlots[NewIndex] = ItemToAdd;

					UpdateInventorySlot(NewIndex);

					// If there are still things leftover to add, recursively call this function.
					int NewRemainder = 0;
					AddItem(NewItem, (LocalAmount - LocalMaxStackAmount), NewRemainder);

					Remainder = NewRemainder;
					return true;
				}
				else // MaxStack amount is greater
				{
					// Add pickup to inventory
					FInventoryItems ItemToAdd;
					ItemToAdd.Item = NewItem; //ItemToAdd.Item = LocalItem->GetClass();
					ItemToAdd.Amount = LocalAmount;
					//InventorySlots.Insert(ItemToAdd, NewIndex);
					InventorySlots[NewIndex] = ItemToAdd;

					UpdateInventorySlot(NewIndex);

					Remainder = 0;
					return true;
				}
			}
			else
			{
				Remainder = LocalAmount;
				return false;
			}
		}

	}
	else
	{
		// Check for empty slot and return the index
		int NewIndex = 0;
		if (CheckForEmptySlot(NewIndex))
		{
			FInventoryItems ItemToAdd;
			ItemToAdd.Item = NewItem;
			ItemToAdd.Amount = 1;//LocalAmount;

			//InventorySlots.Insert(ItemToAdd, NewIndex);
			InventorySlots[NewIndex] = ItemToAdd;

			// Updates inventory widget
			UpdateInventorySlot(NewIndex);

			LocalAmount--;

			if (LocalAmount > 0)
			{
				int NewRemainder = 0;
				AddItem(NewItem, LocalAmount, NewRemainder);

				Remainder = NewRemainder;
				return true;
			}
			else
			{
				Remainder = 0;
				return true;
			}
		}
		else
		{
			Remainder = LocalAmount;
			return false;
		}
	}
}

bool UInventoryComponent::CheckForEmptySlot(int& EmptySlotIndex)
{
	bool bSuccess = false;
	for (int i=0; i<InventorySlots.Num(); i++)
	//for(FInventoryItems CurrentItem : InventorySlots)
	{
		FInventoryItems CurrentItem = InventorySlots[i];

		// If CurrentItem.Item is valid, Slot already has an item.
		if (CurrentItem.Item != nullptr)
		{
			bSuccess = false;
		}
		else // Slot is empty, so there is enough space for a new item!
		{
			bSuccess = true;
			EmptySlotIndex = i;
			break;
		}
	}

	return bSuccess;
}

bool UInventoryComponent::CheckForFreeSlot(TSubclassOf<AInventoryItemMaster>& NewItem, int& OutIndex)
{
	AInventoryItemMaster* PickedUpItem = Cast<AInventoryItemMaster>(NewItem.GetDefaultObject());

	// Success on finding a free slot
	bool bSuccess = false;

	for (int i = 0; i<InventorySlots.Num(); i++)
	{
		// Search in our inventory slot the NewItem and see if it has room in it.
		FInventoryItems CurrentItem = InventorySlots[i];
		// if current item is equal to the NewItem being picked up **AND** if it has room (i.e can be stacked)
		if ( (CurrentItem.Item == NewItem) && (CurrentItem.Amount < PickedUpItem->ItemData.MaxStackAmount))
		{
			bSuccess = true;
			OutIndex = i;
			break;
		}
		else
		{
			bSuccess = false;
		}
	}

	return bSuccess;
}

void UInventoryComponent::GetItemDataAtIndex(int Index, TSubclassOf<AInventoryItemMaster>& OutItem, int& OutAmount)
{
	// Sets the out parameters to be the InventorySlots element at Index
	FInventoryItems InventoryItemInfo = InventorySlots[Index];
	OutItem = InventoryItemInfo.Item;
	OutAmount = InventoryItemInfo.Amount;
	/* 
	* Sets OutAmount to 0 if the InventoryItemInfo.Item hasn't been initialized.
	* If the InventoryItemInfo.Item is not initialized, this check will prevent
	* the OutAmount to be a random/weird number loaded from memory and simply return 0.
	* This helps with the bug where we start with an empty inventory but displays a long amount like '1634002'
	*/
	/*
	if (OutItem.GetDefaultObject())
	{
		OutAmount = InventoryItemInfo.Amount;
	}
	else
	{
		OutAmount = 0;
	}
	*/

	UE_LOG(LogTemp, Error, TEXT("GetItemDataAtIndex: %d, and the amount in this index is: %d"), Index, OutAmount);
}

