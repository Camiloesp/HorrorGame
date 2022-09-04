// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintImplementableEvent)
	UWidgetAnimation* GetFadeinAnimation();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* RestartButton;

	UFUNCTION()
	void RestartButtonPressed();
};
