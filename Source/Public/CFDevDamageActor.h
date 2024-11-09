// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CFDevDamageActor.generated.h"

UCLASS()
class CF_API ACFDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACFDevDamageActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor SphereColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> DamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
