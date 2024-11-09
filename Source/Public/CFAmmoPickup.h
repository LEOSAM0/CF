// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFBasePickup.h"
#include "CFAmmoPickup.generated.h"

class ACFBaseWeapon;

UCLASS()
class CF_API ACFAmmoPickup : public ACFBasePickup
{
	GENERATED_BODY()
	
protected:
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
      int32 ClipsAmount = 10;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
      TSubclassOf<ACFBaseWeapon> WeaponType;


private:
   virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
