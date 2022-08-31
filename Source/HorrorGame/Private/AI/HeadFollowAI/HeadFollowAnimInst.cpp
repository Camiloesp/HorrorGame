// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/HeadFollowAI/HeadFollowAnimInst.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UHeadFollowAnimInst::UHeadFollowAnimInst()
{
	HeadRotation = FRotator(0.f, 0.f, 0.f);
	bCanSeePlayer = false;
	bInterpTowardsPlayer = false;
	MaxHeadTurn = 45.f;
	MinHeadTurn = -45.f;
}

void UHeadFollowAnimInst::InterpDelay()
{
	bInterpTowardsPlayer = true;
}

void UHeadFollowAnimInst::UpdateAnim(float DeltaTime)
{
	// RotateHead
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) // expensive call
	{
		// Get Look at rotation, to determine if player is between a 90degree view (Middle is 0 degrees)
		FVector Start = TryGetPawnOwner()->GetActorLocation();
		FVector Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();// expensive call
		FRotator NewLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);

		// cannot see the player outside our 90 degree range
		bCanSeePlayer = (NewLookAtRotation.Yaw < MaxHeadTurn && NewLookAtRotation.Yaw > MinHeadTurn);
		if (bCanSeePlayer)
		{
			FTimerHandle InterpDelay;
			TryGetPawnOwner()->GetWorld()->GetTimerManager().SetTimer(InterpDelay, this, &UHeadFollowAnimInst::InterpDelay, 1.f, false);
		}
		else
		{
			bInterpTowardsPlayer = false;
		}

		if (bCanSeePlayer) // rotate head if we see player
		{
			// Sets the new head rotation
			float NewYaw = UKismetMathLibrary::ClampAngle(NewLookAtRotation.Yaw, MinHeadTurn, MaxHeadTurn); // can't rotate head 180 degrees
			FRotator NewHeadRotation = FRotator(0.f, 0.f, 0.f);
			NewHeadRotation.Yaw = NewYaw;
			NewHeadRotation.Pitch = NewLookAtRotation.Roll;
			NewHeadRotation.Roll = NewLookAtRotation.Pitch * -1; //looks down when we crouch, up when jump

			// Slowly interpolate head rotation
			int InterpSpeed = bInterpTowardsPlayer ? 5.f : 0.f;
			HeadRotation = UKismetMathLibrary::RInterpTo(HeadRotation, NewHeadRotation, DeltaTime, InterpSpeed);
		}
		else // Rotate head back to 0, or keep head at 0
		{
			//HeadRotation = FRotator(0.f, 0.f, 0.f);
			HeadRotation = UKismetMathLibrary::RInterpTo(HeadRotation, FRotator(0.f, 0.f, 0.f), DeltaTime, 5.f);
		}

		
	}
}
