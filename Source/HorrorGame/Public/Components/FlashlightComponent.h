// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlashlightComponent.generated.h"


class USpotLightComponent;
class AHGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UFlashlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlashlightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/* Flashlight reference */
	UPROPERTY()
	USpotLightComponent* FlashlightLight;

	/* Player that owns this component */
	AHGCharacter* PlayerRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxBatteryLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MinBatteryLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentBatteryLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DepletionAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DepletionSpeed;

	FTimerHandle DepleateTimerHandler;

protected:

	/* 
	* Depleats flashlight life over time
	*/
	void DepleatBatteryLife();

	/* Changes battery life when life is being depleated or life is being added */
	void SetIntensity();

public:

	/*
	* Adds battery life, via using battery in our inventory
	* @param Amount How much battery we are adding
	*/
	void AddBatteryLife(float Amount);

	void ToggleFlashlight();

	void Initialize(AHGCharacter* OwnerRef);

	FORCEINLINE void SetFlashlightLight(USpotLightComponent* NewFlashlightLight) { FlashlightLight = NewFlashlightLight; }

	FORCEINLINE USpotLightComponent* GetFlashlightLight() const { return FlashlightLight; }
	FORCEINLINE float GetCurrentBatteryLevel() const { return CurrentBatteryLevel; }
	FORCEINLINE float GetMaxBatteryLevel() const { return MaxBatteryLevel; }
		
};
