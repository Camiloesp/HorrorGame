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
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxStackAmount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class APickupActorMaster> PickupActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeUsed = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeExamined = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeDropped = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMesh* ExaminationMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExaminationMeshOffset = 0.f;

	/* Initial rotation when examinating an object */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator ExaminationMeshRotation;
};
