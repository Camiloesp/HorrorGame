// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UCameraWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UImage* RecordImageDot;

public:

	/* Get the animation created in WBP */
	UFUNCTION(BlueprintImplementableEvent)
	FORCEINLINE UWidgetAnimation* GetRecImageFlash();
};
