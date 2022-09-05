// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

bool UMainMenu::Initialize()
{
	bool bInit = Super::Initialize();


	PlayButton->OnPressed.AddDynamic(this, &UMainMenu::PlayButtonPressed);
	QuitButton->OnPressed.AddDynamic(this, &UMainMenu::QuitButtonPressed);


	//ResidentEvilButton->OnPressed.AddDynamic(this, &UMainMenu::QuitButtonPressed);
	//FiveNightsAtFreddysButton->OnPressed.AddDynamic(this, &UMainMenu::QuitButtonPressed);
	//HorrorGameButton->OnPressed.AddDynamic(this, &UMainMenu::QuitButtonPressed);

	return bInit;
}

void UMainMenu::PlayButtonPressed()
{
	// Change to level selector
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
