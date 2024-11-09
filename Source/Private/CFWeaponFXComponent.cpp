// Fill out your copyright notice in the Description page of Project Settings.


#include "CFWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "sound/SoundCue.h"

// Sets default values for this component's properties
UCFWeaponFXComponent::UCFWeaponFXComponent()
{
   // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
   // off to improve performance if you don't need them.
   PrimaryComponentTick.bCanEverTick = false;

   // ...
}

void UCFWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
   auto ImpactData = DefaultImpactData;

   if (Hit.PhysMaterial.IsValid())
   {
      const auto PhysMat = Hit.PhysMaterial.Get();
      if (ImpactDataMap.Contains(PhysMat))
      {
         ImpactData = ImpactDataMap[PhysMat];
      }
   }

   // niagara
   UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
      ImpactData.NiagaraEffect,                              //
      Hit.ImpactPoint,                                       //
      Hit.ImpactNormal.Rotation());

   // decal
   auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), //
      ImpactData.DecalData.Material,                 //
      ImpactData.DecalData.Size,                     //
      Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

   if (DecalComponent)
   {
      DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
   }

   //sound
   UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}