// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Kismet/KismetArrayLibrary.h"
//#include "Kismet/GameplayStatics.h"
#include "Controllers/HGPlayerController.h"
#include "GameFramework/Character.h"
#include "Actors/Inventory/InventoryItemMaster.h"

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

			// Adds empty InventoryItems slots
			/*
			for (int i = 0; i<=OwnerController->GetInventorySlots(); i++)
			{
				FInventoryItems NewSlot;
				InventorySlots.Add(NewSlot);
			}
			*/
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

bool UInventoryComponent::AddItem(TSubclassOf<AInventoryItemMaster> Item, int Amount)
{
	AInventoryItemMaster* LocalItem = Cast<AInventoryItemMaster>(Item.Get()); // Item
	int LocalAmount = Amount;
	int LocalMaxStackAmount = LocalItem->ItemData.MaxStackAmount;  //Item->ItemData.MaxStackAmount;

	// Check for empty slot and return the index
	int NewIndex = 0;
	if (CheckForEmptySlot(NewIndex))
	{
		FInventoryItems ItemToAdd;
		ItemToAdd.Item = LocalItem; //ItemToAdd.Item = LocalItem->GetClass();
		ItemToAdd.Amount = LocalAmount;

		InventorySlots.Insert(ItemToAdd, NewIndex);
		return true;
	}
	else
	{
		return false;
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent - No free slots"));
	}
}

bool UInventoryComponent::CheckForEmptySlot(int& EmptySlotIndex)
{
	bool bSuccess = false;
	for (int i=0; i<InventorySlots.Num(); i++)
	//for(FInventoryItems CurrentItem : InventorySlots)
	{
		FInventoryItems CurrentItem = InventorySlots[i];

		if (CurrentItem.Item)
		{
			bSuccess = false;
		}
		else
		{
			bSuccess = true;
			EmptySlotIndex = i;
			break;
		}
	}

	return bSuccess;
}

