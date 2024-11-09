// Fill out your copyright notice in the Description page of Project Settings.


#include "CFBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

// Sets default values
ACFBaseWeapon::ACFBaseWeapon()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = false;

   WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
   SetRootComponent(WeaponMesh);//WeaponMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACFBaseWeapon::BeginPlay()
{
   Super::BeginPlay();

   check(WeaponMesh);

   CurrentAmmo = DefaultAmmo;
}


void ACFBaseWeapon::StartFire()
{

}

void ACFBaseWeapon::StopFire()
{

}

void ACFBaseWeapon::MakeShot()
{

}


bool ACFBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
   const auto CFCharacter = Cast<ACharacter>(GetOwner());
   if (!CFCharacter) return false;

   if (CFCharacter->IsPlayerControlled())
   {
      const auto Controller = CFCharacter->GetController<APlayerController>();
      if (!Controller) return false;

      Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
   }
   else
   {
      ViewLocation = GetMuzzleWorldLocation();
      ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
   }
      
   return true;
}

FVector ACFBaseWeapon::GetMuzzleWorldLocation() const
{
   return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ACFBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
   FVector ViewLocation;
   FRotator ViewRotation;
   if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

   TraceStart = ViewLocation;              // SocketTransform.GetLocation();
   const FVector ShootDirection = ViewRotation.Vector(); // SocketTransform.GetRotation().GetForwardVector();
   TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
   return true;
}

void ACFBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
   if (!GetWorld()) return;
   FCollisionQueryParams CollisionParams;
   CollisionParams.AddIgnoredActor(GetOwner());

   CollisionParams.bReturnPhysicalMaterial = true;

   GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ACFBaseWeapon::DecreaseAmmo()
{
   CurrentAmmo.Bullets--;

   if (IsClipEmpty() && !IsAmmoEmpty())
   {
      StopFire();
      OnClipEmpty.Broadcast(this);
   }
}

bool ACFBaseWeapon::IsAmmoEmpty() const
{
   return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ACFBaseWeapon::IsClipEmpty() const
{
   return CurrentAmmo.Bullets == 0;
}

void ACFBaseWeapon::ChangeClip()
{

   if (!CurrentAmmo.Infinite)
   {
      CurrentAmmo.Clips--;
   }
   CurrentAmmo.Bullets = DefaultAmmo.Bullets;
   //UE_LOG(LogBaseWeapon, Display, TEXT("-------change clip------------"));
}

bool ACFBaseWeapon::CanReload() const
{
   return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool ACFBaseWeapon::IsAmmoFull() const
{
   return CurrentAmmo.Clips == DefaultAmmo.Clips && //
      CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool ACFBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
   if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

   if (IsAmmoEmpty())
   {
      CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
      OnClipEmpty.Broadcast(this);
   }
   else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
   {
      const auto NextClipAmount = CurrentAmmo.Clips + ClipsAmount;
      if (DefaultAmmo.Clips - NextClipAmount >= 0)
      {
         CurrentAmmo.Clips = NextClipAmount;
      }
      else
      {
         CurrentAmmo.Clips = DefaultAmmo.Clips;
         CurrentAmmo.Bullets = DefaultAmmo.Bullets;
      }
   }
   else
   {
      CurrentAmmo.Bullets = DefaultAmmo.Bullets;
   }

   return true;
}

UNiagaraComponent* ACFBaseWeapon::SpawnMuzzleFX()
{
   return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, //
      WeaponMesh,                                        //
      MuzzleSocketName,                                  //
      FVector::ZeroVector,                               //
      FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}