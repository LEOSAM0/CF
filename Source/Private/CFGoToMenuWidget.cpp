// Fill out your copyright notice in the Description page of Project Settings.


#include "CFGoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CFGameInstance.h"

void UCFGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButon)
	{
		GoToMenuButon->OnClicked.AddDynamic(this, &UCFGoToMenuWidget::OnGoToMenu);
	}
}

void UCFGoToMenuWidget::OnGoToMenu()
{
	if (!GetWorld()) return;

	const auto CFGameInstance = GetWorld()->GetGameInstance<UCFGameInstance>();
	if (!CFGameInstance) return;

	if (CFGameInstance->GetMenuLevelName().IsNone())
	{
		return;
	}

	//const FName StartupLevelName = "Training";
	UGameplayStatics::OpenLevel(this, CFGameInstance->GetMenuLevelName());
}