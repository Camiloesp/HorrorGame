// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeadFollowAnimInst.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UHeadFollowAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UHeadFollowAnimInst();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator HeadRotation;

	bool bCanSeePlayer;
	bool bInterpTowardsPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxHeadTurn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MinHeadTurn;

	void InterpDelay();

protected:
public:

	//called every tick from AAIHeadFollow.cpp
	void UpdateAnim(float DeltaTime);

};
