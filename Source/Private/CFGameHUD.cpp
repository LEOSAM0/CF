// Fill out your copyright notice in the Description page of Project Settings.


#include "CFGameHUD.h"
#include "Engine/Canvas.h"
#include "CFBaseWidget.h"
#include "CF/CFGameModeBase.h"
#include "CF_GMB_OnePlayer.h"

DEFINE_LOG_CATEGORY_STATIC(LogCFGameHUD, All, All);

void ACFGameHUD::DrawHUD()
{
   Super::DrawHUD();

   //DrawCrossHair();
}

void ACFGameHUD::BeginPlay()
{
   Super::BeginPlay();

   GameWidgets.Add(ECFMatchState::InProgress, CreateWidget<UCFBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
   GameWidgets.Add(ECFMatchState::Pause, CreateWidget<UCFBaseWidget>(GetWorld(), PauseWidgetClass));
   GameWidgets.Add(ECFMatchState::GameOver, CreateWidget<UCFBaseWidget>(GetWorld(), GameOverWidgetClass));

   for (auto GameWidgetPair : GameWidgets)
   {
      const auto GameWidget = GameWidgetPair.Value;
      if (!GameWidget) continue;

      GameWidget->AddToViewport();
      GameWidget->SetVisibility(ESlateVisibility::Hidden);
   }

   if (GetWorld())
   {
      const auto GameMode = Cast<ACFGameModeBase>(GetWorld()->GetAuthGameMode());
      if (GameMode)
      {
         GameMode->OnMatchStateChanged.AddUObject(this, &ACFGameHUD::OnMatchStateChanged);
      }
   }

   if (GetWorld())
   {
      const auto GameMode = Cast<ACF_GMB_OnePlayer>(GetWorld()->GetAuthGameMode());
      if (GameMode)
      {
         GameMode->OnMatchStateChanged.AddUObject(this, &ACFGameHUD::OnMatchStateChanged);
      }
   }
}

void ACFGameHUD::OnMatchStateChanged(ECFMatchState State)
{
   if (CurrentWidget)
   {
      CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
   }

   if (GameWidgets.Contains(State))
   {
      CurrentWidget = GameWidgets[State];
   }

   if (CurrentWidget)
   {
      CurrentWidget->SetVisibility(ESlateVisibility::Visible);
      CurrentWidget->Show();
   }
   UE_LOG(LogCFGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}

void ACFGameHUD::DrawCrossHair()
{
   const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

   const float HalfLineSize = 10.f;
   const float LineThickness = 2.f;
   const FLinearColor LineColor = FLinearColor::Green;

   DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
   DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
