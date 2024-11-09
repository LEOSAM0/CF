// Fill out your copyright notice in the Description page of Project Settings.


#include "CFAICharacter.h"
#include "CFAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CFAIWeaponComponent.h"
#include "BrainComponent.h"
#include "components/WidgetComponent.h"
#include "CFHealthBarWidget.h"
#include "CFHealthComponent.h"

ACFAICharacter::ACFAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass< UCFAIWeaponComponent>("WeaponComponent"))
{
   AutoPossessAI = EAutoPossessAI::Disabled;
   AIControllerClass = ACFAIController::StaticClass();

   bUseControllerRotationYaw = false;
   if (GetCharacterMovement())
   {
      GetCharacterMovement()->bUseControllerDesiredRotation = true;
      GetCharacterMovement()->RotationRate = FRotator(0.f, 150.f, 0.f);
   }

   HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
   HealthWidgetComponent->SetupAttachment(GetRootComponent());
   HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
   HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ACFAICharacter::BeginPlay()
{
   Super::BeginPlay();

   check(HealthWidgetComponent);
}

void ACFAICharacter::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   UpdateHealthWidgetVisibility();
}

void ACFAICharacter::OnDeath()
{
   Super::OnDeath();

   const auto CFController = Cast<AAIController>(Controller);
   if (CFController && CFController->BrainComponent)
   {
      CFController->BrainComponent->Cleanup();
   }
}

void ACFAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
   Super::OnHealthChanged(Health, HealthDelta);

   const auto HealthBarWidget = Cast<UCFHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
   if (!HealthBarWidget) return;
   HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ACFAICharacter::UpdateHealthWidgetVisibility()
{
   if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) return;

   const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
   const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
   HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}