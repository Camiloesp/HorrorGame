// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryDropDown.generated.h"


class UButton;
class USizeBox;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UInventoryDropDown : public UUserWidget
{
	GENERATED_BODY()
	

public:
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

protected:
public:
};
