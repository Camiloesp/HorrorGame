// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectivesWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UObjectivesWidget : public UUserWidget
{
	GENERATED_BODY()
	

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UTextBlock* ObjectiveText;

protected:

public:

	void UpdateObjectiveText(FText NewText);

};
