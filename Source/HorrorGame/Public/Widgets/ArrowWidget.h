// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArrowWidget.generated.h"

class UButton;
class ALockDial;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UArrowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	UButton* ArrowButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bAddRotation;

	ALockDial* LockDialRef;

protected:

	UFUNCTION()
	void ArrowButtonPressed();

public:

	FORCEINLINE void SetLockDialRef(ALockDial* NewLockDialRef) { LockDialRef = NewLockDialRef; }
	FORCEINLINE void SetAddRotation(bool bNewAddRotation) { bAddRotation = bNewAddRotation; }

};
