// Fill out your copyright notice in the Description page of Project Settings.


#include "CFLauncherWeapon.h"
#include "CFProjectile.h"
#include "sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void ACFLauncherWeapon::StartFire()
{
   MakeShot();
}

void ACFLauncherWeapon::MakeShot()
{
   //if (!GetWorld() || IsAmmoEmpty()) return;
   if (!GetWorld()) return;

   if (IsAmmoEmpty())
   {
      UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
      return;
   }

   FVector TraceStart, TraceEnd;
   if (!GetTraceData(TraceStart, TraceEnd)) return;

   FHitResult HitResult;
   MakeHit(HitResult, TraceStart, TraceEnd);

   const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
   const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

   const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
   ACFProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACFProjectile>(ProjectileClass, SpawnTransform);
   if (Projectile)
   {
      Projectile->SetShotDirection(Direction);
      Projectile->SetOwner(GetOwner());
      Projectile->FinishSpawning(SpawnTransform);
   }
   DecreaseAmmo();
   SpawnMuzzleFX();
   UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);

  /* const UObject* WorldContextObject = nullptr;
   FLatentActionInfo LatentInfo;
   UKismetSystemLibrary::Delay(WorldContextObject, 3.0f, LatentInfo);*/
}
