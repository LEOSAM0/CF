// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFCoreTypes.h"
#include "Components/ActorComponent.h"
#include "CFHealthComponent.generated.h"

class UCameraShakeBase;
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CF_API UCFHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCFHealthComponent();

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
		bool IsDead() { return Health <= 0.f; }

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealthPercent() const { return Health / MaxHealth; }

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;


	FOnDeath OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "1.0", ClampMax = "1000.0"))
		float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
		bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealUpdateTime = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealDelay = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealModifier = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		TMap<UPhysicalMaterial*, float> DamageModifiers;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Health = 0.f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
		void OnTakeAnyDamage(
			AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
		void OnTakePointDamage(AActor* DamageActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
			class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
			AActor* DamageCauser);

	UFUNCTION()
		void OnTakeRadialDamage(AActor* DamageActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo,
			class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);
	void PlayCameraShake();

	void Killed(AController* KillerController);
	void ApplyDamage(float Damage, AController* InstigatedBy);
	float GetPointDamageModifier(AActor* DamageActor, const FName& BoneName);

	void ReportDamageEvent(float Damage, AController* InstigatedBy);
};
