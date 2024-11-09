// Fill out your copyright notice in the Description page of Project Settings.


#include "CFGameOverWidget.h"
#include "CF/CFGameModeBase.h"
#include "CF_GMB_OnePlayer.h"
#include "CFPlayerState.h"
#include "CFPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "CFUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCFGameOverWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   if (GetWorld())
   {
      const auto GameMode = Cast<ACFGameModeBase>(GetWorld()->GetAuthGameMode());
      if (GameMode)
      {
         GameMode->OnMatchStateChanged.AddUObject(this, &UCFGameOverWidget::OnMatchStateChanged);
      }
   }

   if (GetWorld())
   {
      const auto GameMode = Cast<ACF_GMB_OnePlayer>(GetWorld()->GetAuthGameMode());
      if (GameMode)
      {
         GameMode->OnMatchStateChanged.AddUObject(this, &UCFGameOverWidget::OnMatchStateChanged);
      }
   }

   if (ResetLevelButton)
   {
      ResetLevelButton->OnClicked.AddDynamic(this, &UCFGameOverWidget::OnResetLevel);
   }
}

void UCFGameOverWidget::OnMatchStateChanged(ECFMatchState State)
{
   if (State == ECFMatchState::GameOver)
   {
      UpdatePlayerStat();
   }
}

void UCFGameOverWidget::UpdatePlayerStat()
{
   if (!GetWorld() || !PlayerStatBox) return;

   PlayerStatBox->ClearChildren();

   for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
   {
      const auto Controller = It->Get();
      if (!Controller) continue;

      const auto PlayerState = Cast<ACFPlayerState>(Controller->PlayerState);
      if (!PlayerState) continue;

      const auto PlayerStatRowWidget = CreateWidget<UCFPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
      if (!PlayerStatRowWidget) continue;

      PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
      PlayerStatRowWidget->SetKills(CFUtils::TextFromInt(PlayerState->GetKillsNum()));
      PlayerStatRowWidget->SetDeaths(CFUtils::TextFromInt(PlayerState->GetDeathsNum()));
      PlayerStatRowWidget->SetTeam(CFUtils::TextFromInt(PlayerState->GetTeamID()));
      PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
      PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

      PlayerStatBox->AddChild(PlayerStatRowWidget);
   }
}

void UCFGameOverWidget::OnResetLevel()
{
   //const FName CurrentLevelName = "Training";
   const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
   UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}