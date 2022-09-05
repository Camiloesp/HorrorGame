// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/VerticalBox.h"

bool UMainMenu::Initialize()
{
	bool bInit = Super::Initialize();


	PlayButton->OnPressed.AddDynamic(this, &UMainMenu::PlayButtonPressed);
	QuitButton->OnPressed.AddDynamic(this, &UMainMenu::QuitButtonPressed);


	ResidentEvilButton->OnPressed.AddDynamic(this, &UMainMenu::ResidentEvilButtonPressed);
	FiveNightsAtFreddysButton->OnPressed.AddDynamic(this, &UMainMenu::FiveNightsAtFreddysButtonPressed);
	HorrorGameButton->OnPressed.AddDynamic(this, &UMainMenu::HorrorGameButtonPressed);
	BackButton->OnPressed.AddDynamic(this, &UMainMenu::BackButtonPressed);


	LevelSelection->SetVisibility(ESlateVisibility::Collapsed);

	return bInit;
}

void UMainMenu::PlayButtonPressed()
{
	// Change to MainMenuSelection visibility and not able to click it to LevelSelection being visible.
	MainMenuSelection->SetVisibility(ESlateVisibility::Collapsed);
	LevelSelection->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenu::QuitButtonPressed()
{
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}

void UMainMenu::ResidentEvilButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("ResidentEvilLevel"));
}

void UMainMenu::FiveNightsAtFreddysButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("FiveNightsAtFreddys"));
}

void UMainMenu::HorrorGameButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level1"));
}

void UMainMenu::BackButtonPressed()
{
	// Change to LevelSelection visibility and not able to click it to MainMenuSelection being visible.
	LevelSelection->SetVisibility(ESlateVisibility::Collapsed);
	MainMenuSelection->SetVisibility(ESlateVisibility::Visible);
}
