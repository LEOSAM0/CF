// Fill out your copyright notice in the Description page of Project Settings.


#include "CFAmmoPickup.h"
#include "CFHealthComponent.h"
#include "CFWeaponComponent.h"
#include "CFUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ACFAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
   const auto HealthComponent = CFUtils::GetCFPlayerComponent<UCFHealthComponent>(PlayerPawn);
   if (!HealthComponent || HealthComponent->IsDead()) return false;

   const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(PlayerPawn);
   if (!WeaponComponent) return false;

   return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
