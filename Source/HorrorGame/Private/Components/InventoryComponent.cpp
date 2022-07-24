// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Kismet/KismetArrayLibrary.h"
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



			//UKismetArrayLibrary::Array_Resize(InventorySlots, OwnerController->GetInventorySlots());

			
			//FArrayProperty* ArrayProperty = CastField<FArrayProperty>(InventorySlots);
			//UKismetArrayLibrary::GenericArray_Resize(&InventorySlots, ArrayProperty, OwnerController->GetInventorySlots());

			// Adds empty InventoryItems slots
			/*
			for (int i = 0; i<=OwnerController->GetInventorySlots(); i++)
			{
				FInventoryItems NewSlot;
				InventorySlots.Add(NewSlot);
			}
			*/
			UE_LOG(LogTemp, Error, TEXT("UInventoryComponent - Setting InventorySlots to be: %d"), OwnerController->GetInventorySlots());
			UE_LOG(LogTemp, Error, TEXT("UInventoryComponent - InventorySlots before: %d"), InventorySlots.Num());
			InventorySlots.SetNum(OwnerController->GetInventorySlots());

			UE_LOG(LogTemp, Error, TEXT("UInventoryComponent - InventorySlots after: %d"), InventorySlots.Num());

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
	AInventoryItemMaster* LocalItem = Cast<AInventoryItemMaster>(NewItem.GetDefaultObject()); // NewItem
	int LocalAmount = Amount;
	int LocalMaxStackAmount = LocalItem->ItemData.MaxStackAmount;  //NewItem->ItemData.MaxStackAmount;

	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - AddItem, LocalAmount: %d, LocalMaxStackAmount: %d"), LocalAmount, LocalMaxStackAmount);

	// Allow inventory item stacking if MaxStackAmout is > 1
	if (LocalMaxStackAmount > 1)
	{
		// Check for empty slot to stack in and return the index
		int NewIndex = 0;
		if (CheckForFreeSlot(NewItem, NewIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - AddItem: STACKING ITEMS"));

			int CurrentAmountInSlot = 0;
			GetItemDataAtIndex(NewIndex, NewItem, CurrentAmountInSlot);

			CurrentAmountInSlot += LocalAmount;

			if (CurrentAmountInSlot > LocalMaxStackAmount)
			{
				LocalAmount = CurrentAmountInSlot - LocalMaxStackAmount;

				// Add pickup to inventory
				FInventoryItems ItemToAdd;
				ItemToAdd.Item = LocalItem->GetClass();
				ItemToAdd.Amount = LocalMaxStackAmount;
				InventorySlots.Insert(ItemToAdd, NewIndex);

				UpdateInventorySlot(NewIndex);

				int NewRemainder = 0;
				AddItem(NewItem, LocalAmount, NewRemainder);

				Remainder = NewRemainder;
				return true;
			}
			else
			{
				// Add pickup to inventory
				FInventoryItems ItemToAdd;
				ItemToAdd.Item = LocalItem->GetClass();
				ItemToAdd.Amount = CurrentAmountInSlot;
				InventorySlots.Insert(ItemToAdd, NewIndex);

				UpdateInventorySlot(NewIndex);

				Remainder = 0;
				return true;
			}
		}
		else // Not able to stack
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - AddItem: NOT STACKING ITEMS"));

			// check for an empty slot
			if (CheckForEmptySlot(NewIndex))
			{
				// if the Amount of the item being picked up is greater than the MaxStackAmount
				if (LocalAmount > LocalMaxStackAmount)
				{
					// Add pickup to inventory
					FInventoryItems ItemToAdd;
					ItemToAdd.Item = LocalItem->GetClass();
					ItemToAdd.Amount = LocalMaxStackAmount;
					InventorySlots.Insert(ItemToAdd, NewIndex);

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
					ItemToAdd.Item = LocalItem->GetClass();
					ItemToAdd.Amount = LocalAmount;
					InventorySlots.Insert(ItemToAdd, NewIndex);

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
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - Free slots found slots"));
			FInventoryItems ItemToAdd;
			ItemToAdd.Item = LocalItem->GetClass(); //AInventoryItemMaster* PickedUpItem = ItemToAdd.Item.GetDefaultObject();   //ItemToAdd.Item = LocalItem; //ItemToAdd.Item = LocalItem->GetClass();
			ItemToAdd.Amount = 1;//LocalAmount;

			InventorySlots.Insert(ItemToAdd, NewIndex);

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
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - No free slots"));
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
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - CheckForEmptySlot loop, i = %d NO LUCK"), i);
			bSuccess = false;

			//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - FInventoryItems CurrentItem, %s"), *(CurrentItem.Item->GetFName().ToString()));
		}
		else // Slot is empty, so there is enough space for a new item!
		{
			bSuccess = true;
			EmptySlotIndex = i;
			UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - CheckForEmptySlot loop, i = %d FOUND SLOT, BREAKING"), i);
			break;
		}
	}

	FString Log = (bSuccess ? TEXT("True") : TEXT("False"));
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - bSuccess in CheckForEmptySlot = %s"), *Log);
	return bSuccess;
}

bool UInventoryComponent::CheckForFreeSlot(TSubclassOf<AInventoryItemMaster>& NewItem, int& OutIndex)
{
	//if (!NewItem) return false;
	AInventoryItemMaster* PickedUpItem = NewItem.GetDefaultObject();
	//if (PickedUpItem) return false;

	// Success on finding a free slot
	bool bSuccess = false;

	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		// Search in our inventory slot the NewItem and see if it has room in it.
		FInventoryItems CurrentItem = InventorySlots[i];
		// if current item is equal to the NewItem being picked up **AND**
		if ( (CurrentItem.Item == NewItem) && (CurrentItem.Amount < PickedUpItem->ItemData.MaxStackAmount) )
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
}

