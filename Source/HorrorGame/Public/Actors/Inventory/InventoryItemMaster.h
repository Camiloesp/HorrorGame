// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "InventoryItemMaster.generated.h"

class AHGCharacter;

UCLASS()
class HORRORGAME_API AInventoryItemMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryItemMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

protected:

	/* Player that holds the item */
	AHGCharacter* PlayerRef;

public:

	bool bUseItemSuccess;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FItemData ItemData;

	virtual void UseItem();


	FORCEINLINE AHGCharacter* GetPlayerRef() const { return PlayerRef; }

	FORCEINLINE void SetPlayerRef(AHGCharacter* NewPlayerRef) { PlayerRef = NewPlayerRef; }
};
