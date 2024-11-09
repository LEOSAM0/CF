// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFAmmoPickup.h"
#include "CF2AmmoPickup.generated.h"

class ACFBaseWeapon;

UCLASS()
class CF_API ACF2AmmoPickup : public ACFAmmoPickup
{
	GENERATED_BODY()
	
protected:
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
      int32 ClipsAmount2 = 3;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
      TSubclassOf<ACFBaseWeapon> WeaponType2;


private:
   virtual bool GivePickupTo(APawn* PlayerPawn) override;
   virtual bool GivePickupTo2(APawn* PlayerPawn) override;
};
