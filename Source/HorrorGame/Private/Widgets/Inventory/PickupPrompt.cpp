// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/PickupPrompt.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UPickupPrompt::TogglePrompt(bool bCanPickUp)
{
	UTexture2D* NewTexture = (bCanPickUp) ? EPrompt : ArrowPrompt;
	PromptImage->SetBrushFromTexture(NewTexture);
}
