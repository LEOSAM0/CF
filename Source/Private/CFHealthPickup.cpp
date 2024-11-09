// Fill out your copyright notice in the Description page of Project Settings.


#include "CFHealthPickup.h"
#include "CFHealthComponent.h"
#include "CFUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ACFHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
   const auto HealthComponent = CFUtils::GetCFPlayerComponent<UCFHealthComponent>(PlayerPawn);
   if (!HealthComponent) return false;
   return HealthComponent->TryToAddHealth(HealthAmount);
}
