// Fill out your copyright notice in the Description page of Project Settings.


#include "CFSpectatorWidget.h"
#include "CFRespawnComponent.h"
#include "CFUtils.h"

bool UCFSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = CFUtils::GetCFPlayerComponent<UCFRespawnComponent>(GetOwningPlayer());
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}