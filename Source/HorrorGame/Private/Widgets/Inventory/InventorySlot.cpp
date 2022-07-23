// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventorySlot.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Characters/HGCharacter.h"
#include "Components/InventoryComponent.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "Components/Image.h"

UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void UInventorySlot::NativeConstruct()
{
}

bool UInventorySlot::Initialize()
{
	bool bInit = Super::Initialize();

	Index = 0; // will be updated when the inventory grid creates this widget
	Amount = 0;

	return bInit;
}

void UInventorySlot::UpdateSlot()
{
	if (PlayerOwnerRef)
	{
		UInventoryComponent* PlayerOwnerInventory = PlayerOwnerRef->GetInventory();
		if (PlayerOwnerInventory)
		{
			// Out parameters to get idem data from the player inventory component
			TSubclassOf<AInventoryItemMaster> OutInvItem;
			int ItemAmount;
			PlayerOwnerInventory->GetItemDataAtIndex(Index, OutInvItem, ItemAmount);

			AInventoryItemMaster* InvItem = OutInvItem.GetDefaultObject();
			if (InvItem)
			{
				// Updates the image when picking up something
				SlotImage->SetBrushFromTexture(InvItem->ItemData.Icon, true);
			}
		}
	}
}
