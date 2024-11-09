// Fill out your copyright notice in the Description page of Project Settings.


#include "CFProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CFWeaponFXComponent.h"

// Sets default values
ACFProjectile::ACFProjectile()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = false;

   CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
   CollisionComponent->InitSphereRadius(5.f);
   CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
   CollisionComponent->bReturnMaterialOnMove = true;
   SetRootComponent(CollisionComponent);

   MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
   MovementComponent->InitialSpeed = 2000.f;
   MovementComponent->ProjectileGravityScale = 0.5f;

   WeaponFXComponent = CreateDefaultSubobject<UCFWeaponFXComponent>("WeaponFXComponent");
}

// Called when the game starts or when spawned
void ACFProjectile::BeginPlay()
{
   Super::BeginPlay();

   check(MovementComponent);
   check(CollisionComponent);
   check(WeaponFXComponent);

   MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
   CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
   CollisionComponent->OnComponentHit.AddDynamic(this, &ACFProjectile::OnProjectileHit);
   SetLifeSpan(LifeSeconds);
}

void ACFProjectile::OnProjectileHit(
   UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
   if (!GetWorld()) return;

   MovementComponent->StopMovementImmediately();

   UGameplayStatics::ApplyRadialDamage(GetWorld(), //
      DamageAmount,                               //
      GetActorLocation(),                         //
      DamageRadius,                               //
      UDamageType::StaticClass(),                 //
      { GetOwner() },                               //
      this,                                       //
      GetController(),                            //
      DoFullDamage);

   //DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.f);
   WeaponFXComponent->PlayImpactFX(Hit);
   Destroy();
}

AController* ACFProjectile::GetController() const
{
   const auto Pawn = Cast<APawn>(GetOwner());
   return Pawn ? Pawn->GetController() : nullptr;
}