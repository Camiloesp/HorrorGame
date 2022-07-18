// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakes/HeadBobWalk.h"

UHeadBobWalk::UHeadBobWalk()
{
	bSingleInstance = true;
	OscillationDuration = 0.2f;

	RotOscillation.Pitch.Amplitude = 0.3f;
	RotOscillation.Pitch.Frequency = 12.f;
	RotOscillation.Yaw.Amplitude = 0.3f;
	RotOscillation.Yaw.Frequency = 6.f;
	RotOscillation.Roll.Amplitude = 0.2f;
	RotOscillation.Roll.Frequency = 3.f;

	LocOscillation.Z.Amplitude = 0.6f;
	LocOscillation.Z.Frequency = 12.f;
}