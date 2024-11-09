// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFBaseWeapon.h"
#include "CFLauncherWeapon.generated.h"

class ACFProjectile;
class USoundCue;

UCLASS()
class CF_API ACFLauncherWeapon : public ACFBaseWeapon
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<ACFProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* NoAmmoSound;

	virtual void MakeShot() override;
};
