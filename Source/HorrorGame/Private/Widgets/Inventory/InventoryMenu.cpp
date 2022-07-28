// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryMenu.h"
#include "Widgets/Inventory/InventoryGrid.h"
#include "Widgets/Inventory/InventorySlot.h"
#include "Widgets//Inventory/InventoryDropDown.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"


bool UInventoryMenu::Initialize()
{
	bool bInit = Super::Initialize();

	// Pass reference to widgets
	if (InventoryGrid)
	{
		InventoryGrid->SetInventoryMenuReference(this);
	}

	return bInit;
}

void UInventoryMenu::OpenDropDownMenu(UInventorySlot* SlotRef)
{
	if (!SlotRef) return;

	UUniformGridSlot* GridSlot = UWidgetLayoutLibrary::SlotAsUniformGridSlot(SlotRef);

	if (GridSlot)
	{
		FVector2D SlotDesiredSize = SlotRef->GetDesiredSize();

		// Take into consideration the padding from each inventory slot.
		FMargin SlotMargin = InventoryGrid->GetInventoryGridPanel()->SlotPadding;
		int32 SlotPaddingSides = SlotMargin.Left + SlotMargin.Right;
		int32 SlotPaddingTopBot = SlotMargin.Top + SlotMargin.Bottom;

		// Rows and columns start at index 0, so add 1.
		int32 SlotColumn = (GridSlot->Column + 0) * (SlotDesiredSize.X + SlotPaddingSides); // Truncate SlotDesiredSize x and y?
		int32 SlotRow = (GridSlot->Row + 1) * (SlotDesiredSize.Y + SlotPaddingTopBot);

		if (DropDownMenu)
		{
			// Sets DropDown menu location.
			FVector2D NewRenderTranslation = FVector2D(SlotColumn, SlotRow);
			DropDownMenu->SetRenderTranslation(NewRenderTranslation);
		}
	}
}
