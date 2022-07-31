// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxStackAmount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeUsed = true;
};
