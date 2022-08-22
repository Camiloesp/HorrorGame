// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interfaces/Interaction.h"
#include "Lock.generated.h"

class ALockDial;
class UCameraComponent;
class UBoxComponent;
class UWidgetInteractionComponent;

UCLASS()
class HORRORGAME_API ALock : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LockMesh;

	/* Camera the player will see through, when unlocking the lock */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetInteractionComponent* WidgetInteraction;

	// Class reference to the dial to spawn. BP_LockDial
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALockDial> LockDialClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ALockDial*> Dials;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* UnlockAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsUnlocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int NumberOfDials;

	/* The combination for this lock to be unlocked */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<int> LockCombination;

	/* Binds input delegates to interact with this lock's dials */
	void EnablePlayerInput();

	/* Time it takes to blend between cameras */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CameraBlendTime;

	/* True if the player is inside our camera, false otherwise. */
	bool bPlayerLockView;

	/* Displays/hides dials arrows when interacting in lock view */
	void ToggleLockDialsArrowsUI();

	void DestroyLock();

protected:
	void SpawnDials();
	UFUNCTION(BlueprintCallable)
	bool CheckCode();
	UFUNCTION()
	void ExitLockView();

	UFUNCTION()
	void LeftMouseButtonPressed();
	UFUNCTION()
	void LeftMouseButtonReleased();

public:
	/* Check every single time we change a number, if the code is correct. */
	void CheckEveryTurn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector BoxPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector BoxExtent;

	virtual void Interact() override;

};
// TODO: Get reference to player (controller?)