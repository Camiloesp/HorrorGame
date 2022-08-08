// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/ExaminationWidget.h"
#include "Components/InventoryComponent.h"
#include "Actors/Inventory/InventoryItemMaster.h"
#include "Actors/Inventory/Examination.h"
#include "Components/TextBlock.h"
#include "Characters/HGCharacter.h"
#include "Actors/Inventory/Examination.h"

#include "Blueprint/WidgetLayoutLibrary.h"

bool UExaminationWidget::Initialize()
{
	bool bInit = Super::Initialize();

	bRotateMesh = false;
	MouseLocation = FVector2D(FVector2D::ZeroVector);

	return bInit;
}

FReply UExaminationWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!OwnerCharacter) return FReply::Unhandled(); // used to get controller

	bRotateMesh = true;

	APlayerController* OwnerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (OwnerController)
	{
		//FVector2D MouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		float LocationX, LocationY;
		UWidgetLayoutLibrary::GetMousePositionScaledByDPI(OwnerController, LocationX, LocationY);
		MouseLocation = FVector2D(LocationX, LocationY);
	}

	// event was handled
	return FReply::Handled();
}

FReply UExaminationWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bRotateMesh = false;

	return FReply::Handled();
}

FReply UExaminationWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!InventoryCompRef) return FReply::Unhandled(); // used to get examination item

	if (bRotateMesh)
	{
		AExamination* ExaminationActor = InventoryCompRef->GetExaminationActor();
		if (ExaminationActor)
		{
			APlayerController* OwnerController = Cast<APlayerController>(OwnerCharacter->GetController());
			if (OwnerController)
			{
				// new rotation for the examination actor when we move our mouse
				float LocationX, LocationY;
				UWidgetLayoutLibrary::GetMousePositionScaledByDPI(OwnerController, LocationX, LocationY);
				FVector2D MouseRotation = FVector2D(LocationX, LocationY) - MouseLocation;
				MouseRotation.X *= -1; //invert it.
				MouseRotation.Y *= -1; //invert it.

				/* Sensitivity OPTIONAL */
				float Sensitivity = 0.5f;
				MouseRotation.X *= Sensitivity; 
				MouseRotation.Y *= Sensitivity;

				// Sets the new rotation
				FRotator NewRotation = FRotator(0.f, MouseRotation.X, MouseRotation.Y);
				ExaminationActor->GetMeshComponent()->AddWorldRotation(NewRotation);

				//reset mouse location
				float NewLocationX, NewLocationY;
				UWidgetLayoutLibrary::GetMousePositionScaledByDPI(OwnerController, NewLocationX, NewLocationY);
				MouseLocation = FVector2D(NewLocationX, NewLocationY);
			}
		}
	}

	//unhandled because always updating
	return FReply::Unhandled();
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

				// Reset mesh relative rotation to 30,0,0 to avoid shrinking the mesh everytime we spam inspect. (Relative rotation starts at X:30)
				NewMesh->SetRelativeLocation(FVector(30.f, 0.f, 0.f));

				// Set distance from camera using ItemData offset.
				FVector NewDistance = FVector();
				NewDistance.X = (SlotItemRef->ItemData.ExaminationMeshOffset + NewMesh->GetRelativeLocation().X);
				NewDistance.Y = NewMesh->GetRelativeLocation().Y;
				NewDistance.Z = NewMesh->GetRelativeLocation().Z;
				NewMesh->SetRelativeLocation(NewDistance);

				NewMesh->SetRelativeRotation(SlotItemRef->ItemData.ExaminationMeshRotation);

				// Change name and description
				if (ItemNameText && ItemDescriptionText)
				{
					ItemNameText->SetText(FText::FromName(SlotItemRef->ItemData.ItemName)); // ?
					ItemDescriptionText->SetText(SlotItemRef->ItemData.Description); // ?
				}
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
