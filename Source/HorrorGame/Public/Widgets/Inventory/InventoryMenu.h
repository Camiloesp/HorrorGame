// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UInventoryGrid;
class UInventorySlot;
class UInventoryDropDown;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

private:

	/* Grid that holds inventory slots */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UInventoryGrid* InventoryGrid;

	/* Dropdown that is displayed when we select a slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UInventoryDropDown* DropDownMenu;

protected:

public:

	/*
	* Opens slot menu options
	* @param Slot Slot that will open the dropdown menu
	*/
	void OpenDropDownMenu(UInventorySlot* SlotRef);

	FORCEINLINE UInventoryGrid* GetInventoryGrid() const { return InventoryGrid; }
};
