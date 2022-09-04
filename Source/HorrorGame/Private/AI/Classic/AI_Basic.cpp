// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Classic/AI_Basic.h"
#include "Animation/AnimationAsset.h"

// Sets default values
AAI_Basic::AAI_Basic()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAI_Basic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAI_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_Basic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAI_Basic::KillPlayer()
{
	if (!AttackAnimation) return;

	GetMesh()->GetAnimInstance()->Montage_Play(AttackAnimation);
}

