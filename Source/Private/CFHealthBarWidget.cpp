// Fill out your copyright notice in the Description page of Project Settings.


#include "CFHealthBarWidget.h"
#include "components/ProgressBar.h"

void UCFHealthBarWidget::SetHealthPercent(float Percent)
{
	if (!HealthProgressBar) return;

	const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent)) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	HealthProgressBar->SetVisibility(HealthBarVisibility);

	const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
	HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

	HealthProgressBar->SetPercent(Percent);
}