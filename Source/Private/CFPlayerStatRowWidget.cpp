// Fill out your copyright notice in the Description page of Project Settings.


#include "CFPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCFPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (!PlayerNameTextBlock) return;
	PlayerNameTextBlock->SetText(Text);
}

void UCFPlayerStatRowWidget::SetKills(const FText& Text)
{
	if (!KillsTextBlock) return;
	KillsTextBlock->SetText(Text);
}

void UCFPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if (!DeathsTextBlock) return;
	DeathsTextBlock->SetText(Text);
}

void UCFPlayerStatRowWidget::SetTeam(const FText& Text)
{
	if (!TeamTextBlock) return;
	TeamTextBlock->SetText(Text);
}

void UCFPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
	if (!PlayerIndicatorImage) return;
	PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCFPlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
	if (!TeamImage) return;
	TeamImage->SetColorAndOpacity(Color);
}