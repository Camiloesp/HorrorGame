// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/ExaminationWidget.h"
#include "Components/InventoryComponent.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "Actors/Inventory/Examination.h"
#include "Components/TextBlock.h"
#include "Characters/HGCharacter.h"

bool UExaminationWidget::Initialize()
{
	bool bInit = Super::Initialize();

	return bInit;
}

void UExaminationWidget::UpdateWidget(int Index)
{
	if (!InventoryCompRef) return;

	// Get data of the item we want to use at a specific index.
	TSubclassOf<AInventoryItemMaster> ItemClassRef;
	int ItemRefAmount = 0;
	InventoryCompRef->GetItemDataAtIndex(Index, ItemClassRef, ItemRefAmount);


	AInventoryItemMaster* SlotItemRef = Cast<AInventoryItemMaster>(ItemClassRef.GetDefaultObject());
	if (SlotItemRef)
	{
		AExamination* ExaminationActor = InventoryCompRef->GetExaminationActor();
		if (ExaminationActor)
		{
			//Set examinator actor static mesh to new static mesh
			UStaticMeshComponent* NewMesh = ExaminationActor->GetMeshComponent();
			if (NewMesh)
			{
				NewMesh->SetStaticMesh(SlotItemRef->ItemData.ExaminationMesh);

				// Set distance from camera using ItemData offset.
				FVector NewDistance = FVector();
				NewDistance.X = (SlotItemRef->ItemData.ExaminationMeshOffset + NewMesh->GetRelativeLocation().X);
				NewDistance.Y = NewMesh->GetRelativeLocation().Y;
				NewDistance.Z = NewMesh->GetRelativeLocation().Z;
				NewMesh->SetRelativeLocation(NewDistance);

				// Change name and description
				ItemNameText->SetText(FText::FromName(SlotItemRef->ItemData.ItemName));
				ItemDescriptionText->SetText(SlotItemRef->ItemData.Description);
			}
		}
	}
}

void UExaminationWidget::CloseExamination()
{
	// This event is bound to OnReturnButtonPressed in BeginPlay in the InventoryComponent
	if (!OwnerCharacter) return;

	OwnerCharacter->ToggleInventory();

	RemoveFromParent();
}
