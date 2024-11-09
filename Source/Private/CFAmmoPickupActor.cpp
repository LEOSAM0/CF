// Fill out your copyright notice in the Description page of Project Settings.


#include "CFAmmoPickupActor.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

ACFAmmoPickupActor::ACFAmmoPickupActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("Collision");
	CollisionBox->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACFAmmoPickupActor::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACFAmmoPickupActor::OnOverlapEnd);
}

void ACFAmmoPickupActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACFAmmoPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACFAmmoPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "Overlap Begin");
}

void ACFAmmoPickupActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Overlap End");
}

