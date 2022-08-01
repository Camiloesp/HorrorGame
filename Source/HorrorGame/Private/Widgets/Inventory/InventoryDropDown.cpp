// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryDropDown.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Widgets/Inventory/InventorySlot.h"
#include "Characters/HGCharacter.h"
#include "Components/InventoryComponent.h"
#include "Actors/Inventory/InventoryItemMaster.h"

bool UInventoryDropDown::Initialize()
{
	bool bInit = Super::Initialize();

	UseButton->OnReleased.AddDynamic(this, &UInventoryDropDown::UseButtonReleased);
	DropButton->OnReleased.AddDynamic(this, &UInventoryDropDown::DropButtonReleased);

	return bInit;
}

void UInventoryDropDown::UseButtonReleased()
{
	if (!PlayerOwnerRef && PlayerOwnerRef->GetInventory()) return;

	PlayerOwnerRef->GetInventory()->UseItem(SlotIndex);
}

void UInventoryDropDown::DropButtonReleased()
{
	if (!PlayerOwnerRef && PlayerOwnerRef->GetInventory()) return;

	PlayerOwnerRef->GetInventory()->DropItem(SlotIndex);
}

void UInventoryDropDown::UpdateMenu(UInventorySlot* CurrentSlot)
{
	// Get SlotIndex
	SlotIndex = CurrentSlot->GetIndex();
	if (PlayerOwnerRef && PlayerOwnerRef->GetInventory())
	{
		UInventoryComponent* PlayerOwnerInventory = PlayerOwnerRef->GetInventory();

		// Get data from our inventory at SlotIndex
		TSubclassOf<AInventoryItemMaster> ItemClassRef;
		int ItemAmount = 0;
		PlayerOwnerInventory->GetItemDataAtIndex(SlotIndex, ItemClassRef, ItemAmount);

		// Hide sections that we dont need from our dropdown menu for the CurrentSlot
		AInventoryItemMaster* ItemRef = ItemClassRef.GetDefaultObject();
		if (ItemRef)
		{
			// if item CAN'T be used, hide the 'use' section from the dropdown menu.
			ESlateVisibility NewVisibility = (ItemRef->ItemData.bCanBeUsed) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
			UseSection->SetVisibility(NewVisibility);
		}

	}
}
