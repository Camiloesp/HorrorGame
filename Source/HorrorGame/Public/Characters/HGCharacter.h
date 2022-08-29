// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "HGCharacter.generated.h"

class UMovement;
class USpringArmComponent;
class UCameraComponent;
class USpotLightComponent;
class UMainHUD;
class IGrabInterface;
class UInventoryMenu;
class AHGPlayerController;
class UInventoryComponent;
//class UHeadBobWalk;
//class UHeadBobRun;
class UExaminationWidget;
class UFlashlightComponent;
class UHealthComponent;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressedReturn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeftMouseButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeftMouseButtonReleased);//InventoryButtonPressed
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryButtonPressed);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFlashlightComponent* FlashlightComponent;

	/* Our personal inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	/* Our First person movement component: Handles sprint */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMovement* HGMovementComponent;

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

	/* Camera shake according our movement speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> WalkCameraShakeClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> RunCameraShakeClass;

	/* Class reference to the playerHUD */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PlayerInventoryClass;
	/* Widget for the player. Contains Corsshair. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UInventoryMenu* InventoryMenuRef;

	AHGPlayerController* ControllerRef;

	UPROPERTY()
	UExaminationWidget* ExaminationWidget;

	bool bIsHiding;

	bool bCanOpenInventory;

	// Walking sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USoundCue* GrassFootstep;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USoundCue* TileFootstep;
	FTimerHandle WalkingSoundTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CrouchSoundStepsSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float WalkingSoundStepsSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SprintSoundStepsSpeed;

	/* Timeline for opening the locker door */
	//FTimeline FootstepTimeline; // OpenLockerDoorTimeline
	/* Keeps track of where we are in our timeline */
	//UPROPERTY(EditAnywhere)
	//UCurveFloat* CurveFloat;

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

	void ReturnButtonPressed();

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

	void HeadBob();

	void InventoryButtonPressed();

	//void HandleInventoryVisibilityAndInput();

	void PlayFootstep();

	/* When close to an object in front of us, blur the back of the object */
	void UpdateDOF();

public:

	bool bReadingNote;

	UInputComponent* PlayerInputComp;

	void ToggleInventory();

	/*  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsInventoryOpen;

	/* Delegates */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPressedReturn OnReturnButtonPressed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInteractButtonPressed OnInteractButtonPressed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLeftMouseButtonPressed OnLeftMouseButtonPressed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLeftMouseButtonReleased OnLeftMouseButtonReleased;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInventoryButtonPressed OnInventoryButtonPressed;

	/* Called when crouching. */
	UFUNCTION(BlueprintImplementableEvent)
	void ShortenPlayerCapsule();

	UFUNCTION(BlueprintImplementableEvent)
	void LengthenPlayerCapsule();

	//UMainHUD* MainHUDRef;

	FORCEINLINE UInventoryComponent* GetInventory() const { return InventoryComponent; }
	FORCEINLINE UInventoryMenu* GetInventoryMenu() const { return InventoryMenuRef; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UExaminationWidget* GetExaminationWidget() const { return ExaminationWidget; }
	FORCEINLINE USpotLightComponent* GetFlashlight() const { return Flashlight; }
	FORCEINLINE UFlashlightComponent* GetFlashlightComponent() const { return FlashlightComponent; }
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	FORCEINLINE bool IsHiding() const { return bIsHiding; }
	FORCEINLINE bool CanOpenInventory() const { return bCanOpenInventory; }
	FORCEINLINE UMainHUD* GetMainHUDRef() const { return MainHUDRef; }

	FORCEINLINE void SetExaminationWidget(UExaminationWidget* NewExaminationWidget) { ExaminationWidget = NewExaminationWidget; }
	FORCEINLINE void SetFlashlight(USpotLightComponent* NewFlashlight) { Flashlight = NewFlashlight; }
	FORCEINLINE void SetFlashlightComponent(UFlashlightComponent* NewFlashlightComponent) { FlashlightComponent = NewFlashlightComponent; }
	FORCEINLINE void SetIsHiding(bool bHide) { bIsHiding = bHide; }
	FORCEINLINE void SetCanOpenInventory(bool OpenInv) { bCanOpenInventory = OpenInv; }
};
