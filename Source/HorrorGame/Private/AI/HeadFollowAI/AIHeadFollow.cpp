// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/HeadFollowAI/AIHeadFollow.h"
#include "AI/HeadFollowAI/HeadFollowAnimInst.h"

// Sets default values
AAIHeadFollow::AAIHeadFollow()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIHeadFollow::BeginPlay()
{
	Super::BeginPlay();
	
	HeadAnimationInst = Cast<UHeadFollowAnimInst>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AAIHeadFollow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HeadAnimationInst)
	{
		HeadAnimationInst->UpdateAnim(DeltaTime);
	}
}

// Called to bind functionality to input
void AAIHeadFollow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

