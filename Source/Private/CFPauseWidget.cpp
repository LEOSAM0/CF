// Fill out your copyright notice in the Description page of Project Settings.


#include "CFPauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

void UCFPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UCFPauseWidget::OnClearPause);
	}
}

void UCFPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();
}