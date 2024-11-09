// Fill out your copyright notice in the Description page of Project Settings.


#include "CFHealthComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"
#include "CF/CFGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

// Sets default values for this component's properties
UCFHealthComponent::UCFHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}


// Called when the game starts
void UCFHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UCFHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &UCFHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UCFHealthComponent::OnTakeRadialDamage);
	}

}

void UCFHealthComponent::OnTakePointDamage(AActor* DamageActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
	class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
	AActor* DamageCauser)
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamageActor, BoneName);
	ApplyDamage(FinalDamage, InstigatedBy);
}

void UCFHealthComponent::OnTakeRadialDamage(AActor* DamageActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	ApplyDamage(Damage, InstigatedBy);
}

void UCFHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

}

void UCFHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UCFHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

void UCFHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UCFHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

bool UCFHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull())  return false;

	SetHealth(Health + HealthAmount);
	return true;
}

bool UCFHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void UCFHealthComponent::PlayCameraShake()
{
	if (IsDead())  return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UCFHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld()) return;

	const auto GameMode = Cast<ACFGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController, VictimController);
}

float UCFHealthComponent::GetPointDamageModifier(AActor* DamageActor, const FName& BoneName)
{
	const auto Character = Cast<ACharacter>(DamageActor);
	if (!Character ||//
		!Character->GetMesh() ||//
		!Character->GetMesh()->GetBodyInstance(BoneName)) return 1.f;

	const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.f;

	return DamageModifiers[PhysMaterial];
}

void UCFHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;

	UAISense_Damage::ReportDamageEvent(GetWorld(),//
		GetOwner(),//
		InstigatedBy->GetPawn(),//
		Damage,//
		InstigatedBy->GetPawn()->GetActorLocation(),//
		GetOwner()->GetActorLocation());
}