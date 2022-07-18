// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakes/HeadBobRun.h"

UHeadBobRun::UHeadBobRun()
{
	bSingleInstance = true;
	OscillationDuration = 0.2f;

	RotOscillation.Pitch.Amplitude = 0.5f;
	RotOscillation.Pitch.Frequency = 17.f;
	RotOscillation.Yaw.Amplitude = 0.5f;
	RotOscillation.Yaw.Frequency = 8.f;
	RotOscillation.Roll.Amplitude = 0.25f;
	RotOscillation.Roll.Frequency = 5.f;

	LocOscillation.Z.Amplitude = 2.f;
	LocOscillation.Z.Frequency = 17.f;
}