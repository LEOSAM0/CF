// Fill out your copyright notice in the Description page of Project Settings.


#include "CFAIController.h"
#include "CFAICharacter.h"
#include "CFAIPerceptionComponent.h"
#include "CFRespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ACFAIController::ACFAIController()
{
	CFAIPerceptionComponent = CreateDefaultSubobject<UCFAIPerceptionComponent>("CFPerceptionComponent");
	SetPerceptionComponent(*CFAIPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<UCFRespawnComponent>("RespawnComponent");

	bWantsPlayerState = true;
}

void ACFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto CFCharacter = Cast<ACFAICharacter>(InPawn);
	if (CFCharacter)
	{
		RunBehaviorTree(CFCharacter->BehaviorTreeAsset);
	}
}

void ACFAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ACFAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}