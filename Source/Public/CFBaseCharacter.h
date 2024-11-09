// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CFBaseCharacter.generated.h"


class UCFHealthComponent;
class UCFWeaponComponent;
class USoundCue;

UCLASS()
class CF_API ACFBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACFBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCFHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCFWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float LifeSpanOnDeath = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamageVelocity = FVector2D(900.f, 1200.f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamage = FVector2D(10.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		FName MaterialColorName = "Paint Color";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* DeathSound;

	virtual void OnHealthChanged(float Health, float HealthDelta);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnDeath();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void TurnOff() override;
	virtual void Reset() override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		float GetMovementDirection() const;

	void SetPlayerColor(const FLinearColor& Color);

private:
	UFUNCTION()
	virtual void OnGroundLanded(const FHitResult& Hit);
protected:
	
};
