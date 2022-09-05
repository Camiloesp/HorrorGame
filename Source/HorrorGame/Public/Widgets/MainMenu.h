// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

protected:
	UFUNCTION()
	void PlayButtonPressed();
	UFUNCTION()
	void QuitButtonPressed();


	UFUNCTION()
	void ResidentEvilButtonPressed();
	UFUNCTION()
	void FiveNightsAtFreddysButtonPressed();
	UFUNCTION()
	void HorrorGameButtonPressed();
private:

	/* MainMenu options */
	/* Play button will switch widget to the 3 level options: Five nights at freddy's, Resident evil camera style, and main level. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* PlayButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* QuitButton;

	/* Level select options */
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* ResidentEvilButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* FiveNightsAtFreddysButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* HorrorGameButton;
	*/
};
