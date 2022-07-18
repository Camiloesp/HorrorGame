// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Movement.generated.h"

class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORGAME_API UMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/* Reference to the player who owns this component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ACharacter* PlayerOwnerRef;

	/* Speed when sprinting */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;
	/* Speed when walking */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	/* Keeps track of max stamina. Stamina is depletes when we sprint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxStamina;
	/* Keeps track of min stamina. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MinStamina;
	/* Keeps track of our current stamina. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentStamina;

	/* Timer while sprinting. It depletes our stamina.  */
	FTimerHandle DepletesStaminaTimer;

	/* Timer when not sprinting. It Regenerate our stamina.  */
	FTimerHandle RegenerateStaminaTimer;

	/* Crouch variables when crouching button is pressed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CrouchSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CrouchHalfHeight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching;

protected:

	/* depletes stamina while running */
	UFUNCTION()
	void SprintTimer();

	/* Called by timer when we stop sprinting */
	UFUNCTION()
	void RegenerateStamina();

public:

	void Initialize(ACharacter* Character);

	void StartSprint();
	void StopSprint();

	void SetPlayerMaxWalkSpeed(float NewSpeed);

	void StartCrouch();
	void EndCrouch();
};
