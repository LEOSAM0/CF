// Fill out your copyright notice in the Description page of Project Settings.


#include "CFGameDataWidget.h"
#include "CF/CFGameModeBase.h"
#include "CF_GMB_OnePlayer.h"
#include "CFPlayerState.h"


int32 UCFGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetCFGameMode();
	return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 UCFGameDataWidget::GetCurrentRoundNumPlayer() const
{
	const auto GameMode = GetCFGameModePlayer();
	return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 UCFGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetCFGameMode();
	return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 UCFGameDataWidget::GetTotalRoundsNumPlayer() const
{
	const auto GameMode = GetCFGameModePlayer();
	return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 UCFGameDataWidget::GetRoundSecondsRemaining() const
{
	const auto GameMode = GetCFGameMode();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

int32 UCFGameDataWidget::GetRoundSecondsRemainingPlayer() const
{
	const auto GameMode = GetCFGameModePlayer();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

ACFGameModeBase* UCFGameDataWidget::GetCFGameMode() const
{
	return GetWorld() ? Cast<ACFGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ACF_GMB_OnePlayer* UCFGameDataWidget::GetCFGameModePlayer() const
{
	return GetWorld() ? Cast<ACF_GMB_OnePlayer>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ACFPlayerState* UCFGameDataWidget::GetCFPlayerState() const
{
	return GetOwningPlayer() ? Cast<ACFPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}