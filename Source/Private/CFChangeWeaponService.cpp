// Fill out your copyright notice in the Description page of Project Settings.


#include "CFChangeWeaponService.h"
#include "CFWeaponComponent.h"
#include "AIController.h"
#include "CFUtils.h"

UCFChangeWeaponService::UCFChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void UCFChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
		{
			WeaponComponent->NextWeapon();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}