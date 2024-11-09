// Fill out your copyright notice in the Description page of Project Settings.


#include "CFMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CFGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "CFLevelItemWidget.h"
#include "sound/SoundCue.h"

void UCFMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UCFMenuWidget::OnStartGame);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UCFMenuWidget::OnQuitGame);
	}

	InitLevelItems();
}

void UCFMenuWidget::InitLevelItems()
{
	const auto CFGameInstance = GetCFGameInstance();
	if (!CFGameInstance) return;

	checkf(CFGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!"));

	if (!VerticalItemBox) return;
	VerticalItemBox->ClearChildren();

	for (auto LevelData : CFGameInstance->GetLevelsData())
	{
		const auto LevelItemWidget = CreateWidget<UCFLevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if (!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &UCFMenuWidget::OnLevelSelected);

		VerticalItemBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if (CFGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(CFGameInstance->GetLevelsData()[0]);
	}
	else
	{
		OnLevelSelected(CFGameInstance->GetStartupLevel());
	}
}

void UCFMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto CFGameInstance = GetCFGameInstance();
	if (!CFGameInstance) return;

	CFGameInstance->SetStartupLevel(Data);

	for (auto LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}

UCFGameInstance* UCFMenuWidget::GetCFGameInstance() const
{
	if (!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<UCFGameInstance>();
}

void UCFMenuWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void UCFMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != HideAnimation) return;
	
	const auto CFGameInstance = GetCFGameInstance();
	if (!CFGameInstance) return;

	//const FName StartupLevelName = "Training";
	UGameplayStatics::OpenLevel(this, CFGameInstance->GetStartupLevel().LevelName);
}

void UCFMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
