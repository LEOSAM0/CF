// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CFFireService.generated.h"

/**
 * 
 */
UCLASS()
class CF_API UCFFireService : public UBTService
{
	GENERATED_BODY()
	
public:
	UCFFireService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
