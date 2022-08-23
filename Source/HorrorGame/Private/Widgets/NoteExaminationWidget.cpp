// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NoteExaminationWidget.h"
#include "Actors/Notes/NoteMaster.h"
#include "Actors/Notes/NoteExamination.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextRenderComponent.h"
#include "Characters/HGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/HGPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UNoteExaminationWidget::Initialize()
{
	bool bInit = Super::Initialize();

	bRotateMesh = false;
	MouseLocation = FVector2D(FVector2D::ZeroVector);

	return bInit;
}

FReply UNoteExaminationWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

FReply UNoteExaminationWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bRotateMesh = false;

	return FReply::Handled();
}

FReply UNoteExaminationWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//if (!InventoryCompRef) return FReply::Unhandled(); // used to get examination item

	if (bRotateMesh)
	{
		//AExamination* ExaminationActor = InventoryCompRef->GetExaminationActor();
		if (NoteExaminationRef)
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
				NoteExaminationRef->GetMeshComponent()->AddWorldRotation(NewRotation);

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

void UNoteExaminationWidget::DisableReaderInput()
{
	AHGPlayerController* ControllerRef = Cast<AHGPlayerController>(OwnerCharacter->GetController());
	if (!ControllerRef || !CurrentNoteActor) return;

	OwnerCharacter->GetCharacterMovement()->DisableMovement();
	OwnerCharacter->SetCanOpenInventory(false);
	ControllerRef->SetIgnoreLookInput(true);
	ControllerRef->bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(ControllerRef, this);

	CurrentNoteActor->SetActorHiddenInGame(true);
}

void UNoteExaminationWidget::EnableReaderInput()
{
	AHGPlayerController* ControllerRef = Cast<AHGPlayerController>(OwnerCharacter->GetController());
	if (!ControllerRef || !CurrentNoteActor) return;

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OwnerCharacter->SetCanOpenInventory(true);
	ControllerRef->ResetIgnoreLookInput();
	ControllerRef->bShowMouseCursor = false;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(ControllerRef);

	CurrentNoteActor->SetActorHiddenInGame(false);
}

void UNoteExaminationWidget::UpdateWidget(ANoteMaster* NoteActor)
{
	if (!NoteActor || !NoteExaminationRef) return;

	CurrentNoteActor = NoteActor;

	//Bind exit delegates
	OwnerCharacter->OnInventoryButtonPressed.AddDynamic(this, &UNoteExaminationWidget::CloseExamination);
	OwnerCharacter->OnReturnButtonPressed.AddDynamic(this, &UNoteExaminationWidget::CloseExamination);

	// Sets text to the NoteActor interacted with
	FText NewText = NoteActor->GetTextNote();
	FText NewBackText = NoteActor->GetBackTextNote();
	UTextRenderComponent* NoteText = NoteExaminationRef->GetNoteText();
	UTextRenderComponent* BackNoteText = NoteExaminationRef->GetBackNoteText();
	if (NoteText && BackNoteText)
	{
		NoteText->SetText(NewText);
		BackNoteText->SetText(NewBackText);
	}

	DisableReaderInput();
}

void UNoteExaminationWidget::CloseExamination()
{
	//UnBind exit delegates
	OwnerCharacter->OnInventoryButtonPressed.RemoveDynamic(this, &UNoteExaminationWidget::CloseExamination);
	OwnerCharacter->OnReturnButtonPressed.RemoveDynamic(this, &UNoteExaminationWidget::CloseExamination);

	OwnerCharacter->bReadingNote = false;

	EnableReaderInput();
	RemoveFromParent();
}


/*
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); //this
		ControllerRef->ResetIgnoreLookInput();//this
		ControllerRef->bShowMouseCursor = false;//this
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(ControllerRef);//this


		GetCharacterMovement()->DisableMovement();//this
		ControllerRef->SetIgnoreLookInput(true); //this
		ControllerRef->bShowMouseCursor = true;//this
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(ControllerRef, InventoryMenuRef);//this
*/