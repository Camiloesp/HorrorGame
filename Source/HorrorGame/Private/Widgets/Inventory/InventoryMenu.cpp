// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryMenu.h"
#include "Widgets/Inventory/InventoryGrid.h"
#include "Widgets/Inventory/InventorySlot.h"
#include "Widgets//Inventory/InventoryDropDown.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"

#include "Components/Button.h"


bool UInventoryMenu::Initialize()
{
	bool bInit = Super::Initialize();

	// Pass reference to widgets
	if (InventoryGrid)
	{
		InventoryGrid->SetInventoryMenuReference(this);
	}

	CloseDropDownMenuButton->OnPressed.AddDynamic(this, &UInventoryMenu::CloseDropDownMenu);

	return bInit;
}

void UInventoryMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Super::NativeTick(MyGeometry, InDeltaTime);

	// We do this so we dont have to click twice to close the menu
	APlayerController* OwnerController = Cast<APlayerController>(GetOwningPlayerPawn()->GetController());
	if (OwnerController)
	{
		CloseDropDownMenuButton->SetUserFocus(OwnerController);
	}
}

void UInventoryMenu::CloseDropDownMenu()
{
	DropDownMenu->SetVisibility(ESlateVisibility::Collapsed);
	CloseDropDownMenuButton->SetVisibility(ESlateVisibility::Collapsed);
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

		// Rows and columns start at index 0, so add 1. And position the dropdown menu below the inventory slot
		int32 SlotColumn = (GridSlot->Column + 0) * (SlotDesiredSize.X + SlotPaddingSides) + 10.f; // Truncate SlotDesiredSize x and y?
		int32 SlotRow = (GridSlot->Row + 1) * (SlotDesiredSize.Y + SlotPaddingTopBot) + -10.f;

		if (DropDownMenu)
		{
			// Sets DropDown menu location.
			FVector2D NewRenderTranslation = FVector2D(SlotColumn, SlotRow);
			DropDownMenu->SetRenderTranslation(NewRenderTranslation);
			DropDownMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // non clickable when hidden
		}

		if (CloseDropDownMenuButton)
		{
			CloseDropDownMenuButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
