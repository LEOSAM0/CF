// Fill out your copyright notice in the Description page of Project Settings.


#include "CF2AmmoPickup.h"
#include "CFHealthComponent.h"
#include "CFWeaponComponent.h"
#include "CFUtils.h"

bool ACF2AmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
   const auto HealthComponent = CFUtils::GetCFPlayerComponent<UCFHealthComponent>(PlayerPawn);
   if (!HealthComponent || HealthComponent->IsDead()) return false;

   const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(PlayerPawn);
   if (!WeaponComponent) return false;

   return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}

bool ACF2AmmoPickup::GivePickupTo2(APawn* PlayerPawn)
{
   const auto HealthComponent = CFUtils::GetCFPlayerComponent<UCFHealthComponent>(PlayerPawn);
   if (!HealthComponent || HealthComponent->IsDead()) return false;

   const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(PlayerPawn);
   if (!WeaponComponent) return false;

   return WeaponComponent->TryToAddAmmo(WeaponType2, ClipsAmount2);
}