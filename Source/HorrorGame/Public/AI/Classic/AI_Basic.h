// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI_Basic.generated.h"

class ARoomTarget;
//class UAnimationAsset;
class UAnimMontage;

UCLASS()
class HORRORGAME_API AAI_Basic : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Basic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackAnimation;
	//UAnimationAsset* AttackAnimation;

	UFUNCTION()
	void CreateGameOverWidget();

protected:

public:

	void KillPlayer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> RoomTargetClass;

};
