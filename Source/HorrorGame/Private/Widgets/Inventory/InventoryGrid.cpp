// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryGrid.h"
#include "Controllers/HGPlayerController.h"
#include "Characters/HGCharacter.h"
//#include "GameFramework/Character.h"
#include "Widgets/Inventory/InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/KismetMathLibrary.h"

UInventoryGrid::UInventoryGrid(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}


bool UInventoryGrid::Initialize()
{
	bool bInit = Super::Initialize();

	// Starting variables
	SlotsPerRow = 4;


	// Set reference to the player who owns this widget, and get number of inventory slots from its controller.
	int InventorySlots = 0;
	OwnerPlayerRef = Cast<AHGCharacter>(GetOwningPlayerPawn());
	if (OwnerPlayerRef)
	{
		AHGPlayerController* OwnerPlayerRefController = Cast<AHGPlayerController>(OwnerPlayerRef->GetController());
		if (OwnerPlayerRefController)
		{
			InventorySlots = OwnerPlayerRefController->GetInventorySlots();
			// TODO: Set SlotsPerRow part of the controller to be more dynamic.
		}
	}

	// Create inventory slots.
	for (int i = 0; i <= (InventorySlots - 1); i++)
	{
		UInventorySlot* InvSlot = CreateWidget<UInventorySlot>(this, InventorySlotsClass);
		if (InvSlot)
		{
			InvSlot->SetIndex(i);
			SlotsArray.Add(InvSlot);
			if (InventoryGridPanel)
			{
				// Determine which Row
				float Trunc = i/SlotsPerRow;
				int NewRow = UKismetMathLibrary::FTrunc(Trunc); // Positive nums rund down, negative numbers round up
				// Determine which Column
				float Dividend = i;
				float Divisor = SlotsPerRow;
				float Reminder = 0.f;
				UKismetMathLibrary::FMod(Dividend, Divisor, Reminder);
				int NewColumn = UKismetMathLibrary::FTrunc(Reminder);
				// Adds children to grid with its corresponding row and column
				InventoryGridPanel->AddChildToUniformGrid(InvSlot, NewRow, NewColumn);
			}

			//Sets reference to the player owner 
			OwnerPlayerRef = Cast<AHGCharacter>(GetOwningPlayerPawn());
			if (OwnerPlayerRef)
			{
				InvSlot->SetPlayerOwnerRef(OwnerPlayerRef);
			}

			// Sets reference to the InventoryMenu
			if (!InventoryMenuReference)
			{
				InvSlot->SetInventoryMenuReference(InventoryMenuReference);
			}

			// Initialize slot, (i.e. amount text.)
			InvSlot->UpdateSlot();
		}
	}

	return bInit;
}

void UInventoryGrid::AddMoreSlots(int Amount)
{
	if (!InventoryGridPanel) return;

	// inventory slots
	int LocalAmountOfSlots = InventoryGridPanel->GetChildrenCount();

	for (int i = 0; i <= (Amount-1); i++)
	//for(int i=1; i<=Amount; i++)
	{
		UInventorySlot* InvSlot = CreateWidget<UInventorySlot>(this, InventorySlotsClass);
		if (InvSlot)
		{
			// New index after previous slots
			int NewIndex = LocalAmountOfSlots + i;

			InvSlot->SetIndex(NewIndex);
			SlotsArray.Add(InvSlot);
			
			if (InventoryGridPanel)
			{
				// Determine which Row
				float Trunc = NewIndex / SlotsPerRow;
				int NewRow = UKismetMathLibrary::FTrunc(Trunc); // Positive nums rund down, negative numbers round up
				// Determine which Column
				float Dividend = NewIndex;
				float Divisor = SlotsPerRow;
				float Reminder = 0.f;
				UKismetMathLibrary::FMod(Dividend, Divisor, Reminder);
				int NewColumn = UKismetMathLibrary::FTrunc(Reminder);
				// Adds children to grid with its corresponding row and column
				InventoryGridPanel->AddChildToUniformGrid(InvSlot, NewRow, NewColumn);
			}
			
			//Sets reference to the player owner 
			OwnerPlayerRef = Cast<AHGCharacter>(GetOwningPlayerPawn());
			if (OwnerPlayerRef)
			{
				InvSlot->SetPlayerOwnerRef(OwnerPlayerRef);
			}

			// Sets reference to the InventoryMenu
			if (!InventoryMenuReference)
			{
				InvSlot->SetInventoryMenuReference(InventoryMenuReference);
			}

			// Initialize slot, (i.e. amount text.)
			InvSlot->UpdateSlot(); //Crashes when adding new slots
		}
	}
}
