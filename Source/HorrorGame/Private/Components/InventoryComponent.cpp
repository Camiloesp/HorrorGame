// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
//#include "Kismet/GameplayStatics.h"
#include "Controllers/HGPlayerController.h"
#include "GameFramework/Character.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "Characters/HGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Actors/Inventory/Pickups/PickupActorMaster.h"
#include "Widgets/Inventory/InventoryMenu.h"
#include "Widgets/Inventory/InventoryGrid.h"
#include "Widgets/Inventory/InventorySlot.h"
#include "Widgets/Inventory/ExaminationWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Inventory/Examination.h"

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
	PlayerRef = Cast<AHGCharacter>(GetOwner());
	if (PlayerRef)
	{
		AHGPlayerController* OwnerController = Cast<AHGPlayerController>(PlayerRef->GetController());
		if (OwnerController)
		{
			// Sets inventory number of slots to be the owner controller inventory slot number 
			InventorySlots.SetNum(OwnerController->GetInventorySlots());
		}
	}

	// Create Widget ands sets reference to ExaminationWidget
	ExaminationWidget = CreateWidget<UExaminationWidget>(GetWorld(), ExaminationWidgetClass);

	// Gets the examination actor in the level (only one, except the camera). GetAllActorsOfClass is an EXPENSIVE OPERATION, TRY TO USE LESS THIS CLASS.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ExaminationActorClass, OutActors);
	ExaminationActor = Cast<AExamination>(OutActors[0]);
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

void UInventoryComponent::UseItem(int SlotIndex)
{
	// Get data of the item we want to use at a specific index.
	TSubclassOf<AInventoryItemMaster> ItemClassRef;
	int ItemRefAmount = 0;
	GetItemDataAtIndex(SlotIndex, ItemClassRef, ItemRefAmount);

	if (ItemRefAmount > 0)
	{
		// Use item
		AInventoryItemMaster* ItemRef = ItemClassRef.GetDefaultObject();
		if (ItemRef)
		{
			ItemRef->UseItem();
		}
		else
		{
			ItemRef = GetWorld()->SpawnActor<AInventoryItemMaster>(ItemClassRef);
			if (ItemRef)
			{
				ItemRef->UseItem();
			}
		}
		// Remove item after use.
		RemoveItem(SlotIndex);

		// Close dropdown menu after use.
		InventoryMenuRef->CloseDropDownMenu();
	}
}

void UInventoryComponent::RemoveItem(int SlotIndex)
{
	// Get data of the item we want to remove at a specific index.
	TSubclassOf<AInventoryItemMaster> ItemClassRef;
	int ItemRefAmount = 0;
	GetItemDataAtIndex(SlotIndex, ItemClassRef, ItemRefAmount);

	// Determines if we should empty the inventory slot or just reduce the item amount.
	FInventoryItems ItemToAdd;
	if (ItemRefAmount > 1)
	{
		// Empty inventory slot when we remove the last item.
		ItemToAdd.Item = ItemClassRef;
		ItemToAdd.Amount = ItemRefAmount - 1;
	}
	else
	{
		// Empty inventory slot when we remove the last item.
		ItemToAdd.Item = nullptr;
		ItemToAdd.Amount = 0;
	}
	InventorySlots[SlotIndex] = ItemToAdd;
	UpdateInventorySlot(SlotIndex);
}

void UInventoryComponent::DropItem(int SlotIndex)
{
	if (!PlayerRef) return;

	// Get data of the item we want to drop
	TSubclassOf<AInventoryItemMaster> ItemClassRef;
	int LocalAmount = 0;
	GetItemDataAtIndex(SlotIndex, ItemClassRef, LocalAmount);

	if (LocalAmount > 0)
	{
		// Remove item stack from inventory
		for (int i = 0; i < LocalAmount; i++)
		{
			RemoveItem(SlotIndex);
		}

		// Close dropdown menu after removing items
		if (InventoryMenuRef)
		{
			InventoryMenuRef->CloseDropDownMenu();
		}

		// Linetrace where we want the object to drop
		FHitResult HitResult;
		const FVector Start = PlayerRef->GetFollowCamera()->GetComponentLocation();
		const FVector End = PlayerRef->GetActorForwardVector() * 200 + Start; // 200 is how far we want to drop it.
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
		
		// Set the location where to spawn the item dropped.
		const FVector SpawnLocation = (HitResult.bBlockingHit) ? HitResult.Location : HitResult.TraceEnd;
		FTransform SpawnTransform = FTransform();
		SpawnTransform.SetLocation(SpawnLocation);

		// Drop, Spawn, actor.
		AInventoryItemMaster* ItemToDrop = ItemClassRef.GetDefaultObject();
		APickupActorMaster* ItemSpawned = nullptr;
		if (ItemToDrop)
		{
			ItemSpawned = GetWorld()->SpawnActor<APickupActorMaster>(ItemToDrop->ItemData.PickupActor, SpawnTransform);

			// Set the dropped item Amount to be the same amount that was in the inventory
			if (ItemSpawned)
			{
				ItemSpawned->Amount = LocalAmount;
				ItemSpawned->GetMesh()->SetSimulatePhysics(true);
			}
		}
	}
	else
	{

	}
}

void UInventoryComponent::GetItemDataAtIndex(int Index, TSubclassOf<AInventoryItemMaster>& OutItem, int& OutAmount)
{
	// Sets the out parameters to be the InventorySlots element at Index
	FInventoryItems InventoryItemInfo = InventorySlots[Index];
	OutItem = InventoryItemInfo.Item;
	OutAmount = InventoryItemInfo.Amount;

	UE_LOG(LogTemp, Error, TEXT("GetItemDataAtIndex: %d, and the amount in this index is: %d"), Index, OutAmount);
}

