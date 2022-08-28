// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ObjectivesWidget.h"
#include "Components/TextBlock.h"

void UObjectivesWidget::UpdateObjectiveText(FText NewText)
{
	ObjectiveText->SetText(NewText);
}
