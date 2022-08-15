// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lock.generated.h"

class ALockDial;

UCLASS()
class HORRORGAME_API ALock : public AActor
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

	// Class reference to the dial to spawn. BP_LockDial
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALockDial> LockDialClass;

	TArray<ALockDial*> Dials;

protected:
public:
};
