// Fill out your copyright notice in the Description page of Project Settings.


#include "CFRifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
//#include "DamageEvents.h"
#include "Engine/EngineTypes.h"
#include "CFWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "components/audiocomponent.h"

ACFRifleWeapon::ACFRifleWeapon()
{
   WeaponFXComponent = CreateDefaultSubobject<UCFWeaponFXComponent>("WeaponFXComponent");
}

void ACFRifleWeapon::BeginPlay()
{
   Super::BeginPlay();

   check(WeaponFXComponent);
}

void ACFRifleWeapon::StartFire()
{
   InitFX();
   GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ACFRifleWeapon::MakeShot, TimeBetweenShots, true);
   MakeShot();
}

void ACFRifleWeapon::StopFire()
{
   GetWorldTimerManager().ClearTimer(ShotTimerHandle);
   SetFXActive(false);
}

void ACFRifleWeapon::MakeShot()
{
   if (!GetWorld() || IsAmmoEmpty())
   {
      StopFire();
      return;
   }

   FVector TraceStart, TraceEnd;
   if (!GetTraceData(TraceStart, TraceEnd))
   {
      StopFire();
      return;
   }

   FHitResult HitResult;
   MakeHit(HitResult, TraceStart, TraceEnd);

   FVector TraceFXEnd = TraceEnd;

   if (HitResult.bBlockingHit)
   {
      TraceFXEnd = HitResult.ImpactPoint;

      MakeDamage(HitResult);

      WeaponFXComponent->PlayImpactFX(HitResult);
   }

   SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
   DecreaseAmmo();
}

bool ACFRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
   FVector ViewLocation;
   FRotator ViewRotation;
   if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

   TraceStart = ViewLocation; // SocketTransform.GetLocation();

   const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
   const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad); // SocketTransform.GetRotation().GetForwardVector();
   TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
   return true;
}

void ACFRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
   const auto DamagedActor = HitResult.GetActor();
   if (!DamagedActor) return;

   FPointDamageEvent PointDamageEvent;
   PointDamageEvent.HitInfo = HitResult;
   DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);//FDamageEvent()
}

void ACFRifleWeapon::InitFX()
{
   if (!MuzzleFXComponent)
   {
      MuzzleFXComponent = SpawnMuzzleFX();
   }

   if (!FireAudioComponent)//FireAudioComponent
   {
      FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
   }
   SetFXActive(true);
}

void ACFRifleWeapon::SetFXActive(bool IsActive)
{
   if (MuzzleFXComponent)
   {
      MuzzleFXComponent->SetPaused(!IsActive);
      MuzzleFXComponent->SetVisibility(IsActive, true);
   }

   if (FireAudioComponent)
   {
      IsActive ? FireAudioComponent->Play() : FireAudioComponent->Stop();
   }
}

void ACFRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
   const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
   if (TraceFXComponent)
   {
      TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
   }
}

AController* ACFRifleWeapon::GetController() const
{
   const auto Pawn = Cast<APawn>(GetOwner());
   return Pawn ? Pawn->GetController() : nullptr;
}

void ACFRifleWeapon::Zoom(bool Enabled)
{
   const auto Controller = Cast<APlayerController>(GetController());
   if (!Controller || !Controller->PlayerCameraManager) return;

   if (Enabled)
   {
      DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
   }

   Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}