// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventorySlot.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Characters/HGCharacter.h"
#include "Components/InventoryComponent.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Widgets/Inventory/InventoryMenu.h"

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

	AmountText->SetVisibility(ESlateVisibility::Hidden);

	SlotButton->OnReleased.AddDynamic(this, &UInventorySlot::SlotButtonPressed);
	SlotButton->OnHovered.AddDynamic(this, &UInventorySlot::SlotButtonHovered);
	SlotButton->OnUnhovered.AddDynamic(this, &UInventorySlot::SlotButtonUnHovered);

	//SlotButton->OnReleased.IsBound();

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
			PlayerOwnerInventory->GetItemDataAtIndex(Index, OutInvItem, Amount);

			// Disabling empty inventory slots.
			if (OutInvItem)
			{
				SlotButton->SetIsEnabled(true);
			}
			else
			{
				SlotButton->SetIsEnabled(false);
				if (EmptyIcon)
				{
					SlotImage->SetBrushFromTexture(EmptyIcon, true);
				}
				AmountText->SetVisibility(ESlateVisibility::Hidden);
			}


			AInventoryItemMaster* InvItem = OutInvItem.GetDefaultObject();
			if (InvItem)
			{
				// Updates the image when picking up something
				SlotImage->SetBrushFromTexture(InvItem->ItemData.Icon, true);
			}

			// Sets the amount text in the widget slot.
			FText ItemAmountText = UKismetTextLibrary::Conv_Int64ToText(Amount, false, true);
			//FText ItemAmountText = FText::FromString(FString::FromInt(Amount));
			AmountText->SetText(ItemAmountText);

			// If we have MORE THAN 1, show text
			ESlateVisibility NewVisibility = (Amount > 1) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
			AmountText->SetVisibility(NewVisibility);
			
		}
	}
}

bool UInventorySlot::HasBeenInitialized()
{
	return SlotButton->OnReleased.IsBound();
}

void UInventorySlot::SlotButtonPressed()
{
	if (!PlayerOwnerRef) return;

	// Get data of the item we want to use at a specific index.
	TSubclassOf<AInventoryItemMaster> ItemClassRef;
	int ItemRefAmount = 0;
	PlayerOwnerRef->GetInventory()->GetItemDataAtIndex(Index, ItemClassRef, ItemRefAmount);

	// Get item data
	AInventoryItemMaster* ItemRef = ItemClassRef.GetDefaultObject();
	if (ItemRef)
	{
		// If we can't use, examine, and drop then do not open the menu
		bool bCanBeUsed = ItemRef->ItemData.bCanBeUsed;
		bool bCanBeExamined = ItemRef->ItemData.bCanBeExamined;
		bool bCanBeDropped = ItemRef->ItemData.bCanBeDropped;
		if (!bCanBeUsed && !bCanBeExamined && !bCanBeDropped)
		{
			// can't do anything with this item, do not open drop down menu
			return;
		}
	}


	if (InventoryMenuReference)
	{
		InventoryMenuReference->OpenDropDownMenu(this);
	}
	else
	{
		if (PlayerOwnerRef)
		{
			// If we have no reference to the InventoryMenuReference, try to get it and call this function again
			InventoryMenuReference = PlayerOwnerRef->GetInventoryMenu();
			SlotButtonPressed();
		}
	}
}

void UInventorySlot::SlotButtonHovered()
{
	if (!PlayerOwnerRef) return;
	if (!InventoryMenuReference)
	{
		InventoryMenuReference = PlayerOwnerRef->GetInventoryMenu();
	}
	FItemData DataToDisplay;

	// Get data of the item we want to display it
	TSubclassOf<AInventoryItemMaster> ItemClassRef;
	int ItemRefAmount = 0;
	PlayerOwnerRef->GetInventory()->GetItemDataAtIndex(Index, ItemClassRef, ItemRefAmount);

	// make sure inventory item is valid and we can get data from that item
	AInventoryItemMaster* ItemRef = Cast<AInventoryItemMaster>(ItemClassRef.GetDefaultObject());
	if (ItemRef)
	{
		DataToDisplay = ItemRef->ItemData;
	}
	else
	{
		return;
	}

	// display data.
	InventoryMenuReference->ShowItemInfo(DataToDisplay);
}

void UInventorySlot::SlotButtonUnHovered()
{
	if (!InventoryMenuReference && !PlayerOwnerRef) return;

	InventoryMenuReference->HideItemInfo();
}
