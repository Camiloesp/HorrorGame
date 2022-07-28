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

void UInventorySlot::SlotButtonPressed()
{
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