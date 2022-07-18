// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HGCharacter.generated.h"

class UMovement;
class USpringArmComponent;
class UCameraComponent;
class USpotLightComponent;
class UMainHUD;
class IGrabInterface;

UCLASS()
class HORRORGAME_API AHGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/* Camera boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/* main camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/* Flashlight */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* Flashlight;

	/* Our First person movement component: Handles sprint */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMovement* HGMovementComp;

	/* Rate speed when we turn using the mouse */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TurnRate;

	/* Distance to interact with interactables */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DistanceToInteract;

	/* Widget for the player. Contains Corsshair. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMainHUD* MainHUDRef;
	/* Class reference to the playerHUD */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PlayerHUDClass;

	/* Current grabbed actor */
	IGrabInterface* GrabbedActor;

protected:

	/* look around */
	void LookUp(float Value);
	void LookRight(float Value);

	/* Movement */
	void MoveForward(float Value);
	void MoveRight(float Value);

	/* Action button pressed */
	void ActionButtonPressed();

	/* Left mouse button pressed/released. (grab) */
	void LeftMouseButtonPressed();
	void LeftMouseButtonReleased();


	/**
	* Tracing a line from center of camera to the world.
	* @param Length Length of the line trace.
	* @return Returns the hit actor by the line trace.
	*/
	AActor* LineTrace(float Length);

	/* called in BeginPlay, to initialize widgets and variables. */
	void Initialize();

	/* Toggles flashlight between on and off. */
	void ToggleFlashlight();

	void Sprint();
	void StopSprint();

	void CrouchButtonPressed();
	void CrouchButtonReleased();

public:

	/* Called when crouching. */
	UFUNCTION(BlueprintImplementableEvent)
	void ShortenPlayerCapsule();

	UFUNCTION(BlueprintImplementableEvent)
	void LengthenPlayerCapsule();

};
