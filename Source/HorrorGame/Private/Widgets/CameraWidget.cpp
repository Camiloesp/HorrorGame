// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CameraWidget.h"
#include "Components/Image.h"

bool UCameraWidget::Initialize()
{
	bool bSuccess = Super::Initialize();

	
	PlayAnimation(GetRecImageFlash(), 0.f, 0.f);

	return bSuccess;
}