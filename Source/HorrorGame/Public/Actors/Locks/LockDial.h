// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "LockDial.generated.h"

UCLASS()
class HORRORGAME_API ALockDial : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockDial();

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
	UStaticMeshComponent* LockDialMesh;

	/* Position of the dial in our lock */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Index;

	/* 
	* Number, 0 to 9, that is being displayed. (According to rotation). 
	* We have 10 sides, so every number is based of 360/10, which is 36.
	* 0 = Rotation X(Roll) = 0.f OR 360.f but if 360 we then set to 0
	* 1 = Rotation X(Roll) = 36.f
	* 2 = Rotation X(Roll) = 72.f
	* 3 = Rotation X(Roll) = 108.f
	* 4 = Rotation X(Roll) = 144.f
	* 5 = Rotation X(Roll) = 180.f
	* 6 = Rotation X(Roll) = 216.f
	* 7 = Rotation X(Roll) = 252.f
	* 8 = Rotation X(Roll) = 288.f
	* 9 = Rotation X(Roll) = 324.f
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Number;

	/* Actual rotation of the dial */
	float DialRotation;

	bool bCanRotate;

	/* Timeline for opening the locker door */
	FTimeline RotateDialTL; // OpenLockerDoorTimeline
	/* Keeps track of where we are in our timeline */
	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

	UFUNCTION()
	void TimelineFinished();

protected:

	/* Starts dial at random number */
	void SetRandomNumber();

public:

	/* Sets the current Number to be the current selection */
	void FindNumber();

	UFUNCTION(BlueprintCallable) //debug
	void RotateDial();
	UFUNCTION()
	void InterpDialRotation(float Value);


	FORCEINLINE int GetIndex() const { return Index; }
	FORCEINLINE int GetNumber() const { return Number; }

	FORCEINLINE void SetIndex(int NewIndex) { Index = NewIndex; }

};
