// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameOverWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

bool UGameOverWidget::Initialize()
{
	bool bSuccess = Super::Initialize();

	PlayAnimation(GetFadeinAnimation());

	RestartButton->OnReleased.AddDynamic(this, &UGameOverWidget::RestartButtonPressed);

	return bSuccess;
}

void UGameOverWidget::RestartButtonPressed()
{
	FName CurrentLevelName = FName( UGameplayStatics::GetCurrentLevelName(GetWorld()) );
	UGameplayStatics::OpenLevel(GetWorld(), CurrentLevelName);
}
