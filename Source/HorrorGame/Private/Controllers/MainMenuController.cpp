// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/MainMenuController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/MainMenu.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

AMainMenuController::AMainMenuController()
{
	SetShowMouseCursor(true);
}

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	UMainMenu* MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuWidgetClass);
	if (!MainMenuWidget) return;

	MainMenuWidget->AddToViewport();

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, MainMenuWidget);

	// Get camera to set view target
	TArray<AActor*> CameraActors; // Only one in MainMenu
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);
	SetViewTargetWithBlend(CameraActors[0]);
}
