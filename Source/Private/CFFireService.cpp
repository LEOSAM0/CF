// Fill out your copyright notice in the Description page of Project Settings.


#include "CFFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "CFUtils.h"
#include "CFWeaponComponent.h"

UCFFireService::UCFFireService()
{
	NodeName = "Fire";
}

void UCFFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (Controller)
	{
		const auto WeaponComponent = CFUtils::GetCFPlayerComponent<UCFWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}