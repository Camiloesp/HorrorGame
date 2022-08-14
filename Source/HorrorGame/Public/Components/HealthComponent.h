// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UMaterialParameterCollectionInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MinHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	/* Material parameter collection that holds the values that handle params for the ScreenBlood material. Set in the character BP */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollection* MaterialParameter;

protected:
public:

	/* 
	* Removes health due to damage taken
	* @param Amount The amount that needs to be taken from CurrentHealth
	*/
	UFUNCTION(BlueprintCallable)
	void RemoveHealth(float Amount);

	/*
	* Add health
	* @param Amount The amount that needs to be added to CurrentHealth
	*/
	UFUNCTION(BlueprintCallable)
	void AddHealth(float Amount);

	void UpdateBloodScreen();

	FORCEINLINE float GetMinHealth() const { return MinHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

};
