// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

class UProgressBars;
class UInventoryGrid;
class AHGCharacter;

/**
 * 
 */
UCLASS()
class HORRORGAME_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;

	virtual bool Initialize() override;


private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AHGCharacter* PlayerOwner;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UInventoryGrid* InventoryGrid;

	/* The health, stamina, and flashlight battery progress bars widget*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UProgressBars* ProgressBars;

protected:
public:

	FORCEINLINE void SetPlayerOwner(AHGCharacter* NewPlayerOwner) { PlayerOwner = NewPlayerOwner; }

	FORCEINLINE AHGCharacter* GetPlayerOwner() const { return PlayerOwner; }
	FORCEINLINE UProgressBars* GetProgressBars() const { return ProgressBars; }
};
