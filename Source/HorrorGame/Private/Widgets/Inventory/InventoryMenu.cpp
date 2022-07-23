// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryMenu.h"
#include "Widgets/Inventory/InventoryGrid.h"
#include "Widgets/Inventory/InventorySlot.h"


bool UInventoryMenu::Initialize()
{
	bool bInit = Super::Initialize();

	/*
	TArray<UInventorySlot*> SlotsArray = InventoryGrid->GetSlotsArray();
	for (int i = 0; i< SlotsArray.Num(); i++)
	{
		UInventorySlot* Slot = SlotsArray[i];
		if (Slot)
		{
			Slot->Initialize();
		}
	}
	*/
	return bInit;
}