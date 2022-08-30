// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interaction.h"
#include "SecurityCameraController.generated.h"

class ASecurityCamera;

UCLASS()
class HORRORGAME_API ASecurityCameraController : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecurityCameraController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/* Root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ComputerMesh;

	bool bIsViewingCameras;

	APlayerController* InteractingPlayerController;

	/* Reference to the BP_SecurityCamera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASecurityCamera> SecurityCameraClass;

	TArray<AActor*> SecurityCameras;

	int CurrentCameraIndex;

protected:

	UFUNCTION()
	void LeaveCameraController();

	/* Changes to another camera view */
	UFUNCTION()
	void ViewThroughCamera();

public:

	virtual void Interact() override;

};
