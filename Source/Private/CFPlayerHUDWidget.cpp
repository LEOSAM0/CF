// Fill out your copyright notice in the Description page of Project Settings.


#include "CFPlayerHUDWidget.h"
#include "CFHealthComponent.h"
#include "CFWeaponComponent.h"
#include "CFUtils.h"
#include "components/ProgressBar.h"
#include "CFPlayerState.h"

void UCFPlayerHUDWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   if (GetOwningPlayer())
   {
      GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UCFPlayerHUDWidget::OnNewPawn);
      OnNewPawn(GetOwningPlayerPawn());
   }
}

void UCFPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
   const auto HealthComponent = CFUtils::GetCFPlayerComponent<UCFHealthComponent>(NewPawn);
   if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
   {
      HealthComponent->OnHealthChanged.AddUObject(this, &UCFPlayerHUDWidget::OnHealthChanged);
   }
   UpdateHealthBar();
}

void UCFPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
   if (HealthDelta < 0.f)
   {
      OnTakeDamage();

      if (!IsAnimationPlaying(DamageAnimation))
      {
         PlayAnimation(DamageAnimation);
      }
   }

   UpdateHealthBar();
}

float UCFPlayerHUDWidget::GetHealthPercent() const
{
   const auto HealthComponent = CFUtils::GetCFPlayerComponent<UCFHealthComponent>(GetOwningPlayerPawn());
   if (!HealthComponent) return 0.0f;

   return HealthComponent->GetHealthPercent();
}

bool UCFPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
   const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(GetOwningPlayerPawn());
   if (!WeaponComponent) return false;

   return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UCFPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
   const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(GetOwningPlayerPawn());
   if (!WeaponComponent) return false;

   return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}


bool UCFPlayerHUDWidget::IsPlayerAlive() const
{
   const auto HealthComponent = CFUtils::GetCFPlayerComponent<UCFHealthComponent>(GetOwningPlayerPawn());
   return HealthComponent && !HealthComponent->IsDead();
}

bool UCFPlayerHUDWidget::IsPlayerSpectating() const
{
   const auto Controller = GetOwningPlayer();
   return Controller && Controller->GetStateName() == NAME_Spectating;
}

int32 UCFPlayerHUDWidget::GetKillsNum() const
{
   const auto Controller = GetOwningPlayer();
   if (!Controller) return 0;

   const auto PlayerState = Cast<ACFPlayerState>(Controller->PlayerState);
   return PlayerState ? PlayerState->GetKillsNum() : 0;
}

void UCFPlayerHUDWidget::UpdateHealthBar()
{
   if (HealthProgressBar)
   {
      HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
   }
}

FString UCFPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
   const int32 MaxLen = 3;
   const TCHAR PrefixSymbol = '0';

   auto BulletStr = FString::FromInt(BulletsNum);
   const auto SymbolNumToAdd = MaxLen - BulletStr.Len();

   if (SymbolNumToAdd > 0)
   {
      BulletStr = FString::ChrN(SymbolNumToAdd, PrefixSymbol).Append(BulletStr);
   }

   return BulletStr;
}