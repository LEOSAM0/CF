// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFBasePickup.h"
#include "CFHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class CF_API ACFHealthPickup : public ACFBasePickup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
		float HealthAmount = 100.f;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
