// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Kismet/KismetArrayLibrary.h"
//#include "Kismet/GameplayStatics.h"
#include "Controllers/HGPlayerController.h"
#include "GameFramework/Character.h"
#include "Actors/Inventory/InventoryItemMaster.h"
//#include "Characters/HGCharacter.h"

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

bool UInventoryComponent::AddItem(TSubclassOf<AInventoryItemMaster> Item, int Amount)
{
	AInventoryItemMaster* LocalItem = Cast<AInventoryItemMaster>(Item.GetDefaultObject()); // Item
	int LocalAmount = Amount;
	int LocalMaxStackAmount = LocalItem->ItemData.MaxStackAmount;  //Item->ItemData.MaxStackAmount;

	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - AddItem, LocalAmount: %d, LocalMaxStackAmount: %d"), LocalAmount, LocalMaxStackAmount);

	// Check for empty slot and return the index
	int NewIndex = 0;
	if (CheckForEmptySlot(NewIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - Free slots found slots"));
		FInventoryItems ItemToAdd;
		ItemToAdd.Item = LocalItem->GetClass(); //AInventoryItemMaster* PickedUpItem = ItemToAdd.Item.GetDefaultObject();   //ItemToAdd.Item = LocalItem; //ItemToAdd.Item = LocalItem->GetClass();
		ItemToAdd.Amount = LocalAmount;

		InventorySlots.Insert(ItemToAdd, NewIndex);

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - No free slots"));
		return false;
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

