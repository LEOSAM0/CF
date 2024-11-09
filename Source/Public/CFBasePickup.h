// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CFBasePickup.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class CF_API ACFBasePickup : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACFBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		float RespawnTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* PickupTakenSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool CouldBeTaken() const;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn);
	virtual bool GivePickupTo2(APawn* PlayerPawn);
	void PickupWasTaken();
	void Respawn();

	float RotationYaw = 0.f;
	void GenerateRotationYaw();
	FTimerHandle RespawnTimerHandle;
};
