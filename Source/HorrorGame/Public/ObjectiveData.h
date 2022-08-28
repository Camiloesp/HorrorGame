// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ObjectiveData.generated.h"

USTRUCT(BlueprintType)
struct FObjectiveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName IDName = FName();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ObjectiveTitle = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bObjectiveCompleted = false;
};
