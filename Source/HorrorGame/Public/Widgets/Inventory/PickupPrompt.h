// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupPrompt.generated.h"

class UImage;
class UTexture2D;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UPickupPrompt : public UUserWidget
{
	GENERATED_BODY()
	

private:

	/* Arrow image. Displayed when player is close to a pickup item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* PromptImage;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTexture2D* ArrowPrompt;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTexture2D* EPrompt;

protected:
public:

	void TogglePrompt(bool bCanPickUp);

};
