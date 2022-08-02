// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExaminationWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UExaminationWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UTextBlock* ItemNameText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UTextBlock* ItemDescriptionText;

	/* Image used to detect the user mouse input to rotate the examination actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* DragDetection;

protected:
public:
};
