// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryDropDown.generated.h"


class UButton;
class USizeBox;
class AHGCharacter;
class UInventorySlot;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UInventoryDropDown : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual bool Initialize() override;

protected:
	
private:

	/* Button to use the selected item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* UseButton;
	/* Button to display item info */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* ExamineButton;
	/* Button to drop the selected item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* DropButton;

	/* Button to use the selected item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	USizeBox* UseSection;
	/* Button to display item info */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	USizeBox* ExamineSection;
	/* Button to drop the selected item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	USizeBox* DropSection;

	AHGCharacter* PlayerOwnerRef;

	/* Slot index the dropdown menu is accessing */
	int SlotIndex = 0;

protected:

	UFUNCTION()
	void UseButtonReleased();

public:

	/*
	* Determines if we need to remove the 'use' 'examine' or 'drop' from the menu
	* @param CurrentSlot Slot reference needed to know how the dropdownmenu will look.
	*/
	void UpdateMenu(UInventorySlot* CurrentSlot);

	FORCEINLINE AHGCharacter* GetPlayerOwnerRef() const { return PlayerOwnerRef; }

	FORCEINLINE void SetPlayerOwnerRef(AHGCharacter* NewPlayerOwnerRef) { PlayerOwnerRef = NewPlayerOwnerRef; }
};
