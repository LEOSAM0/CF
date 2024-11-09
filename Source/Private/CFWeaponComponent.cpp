// Fill out your copyright notice in the Description page of Project Settings.


#include "CFWeaponComponent.h"
#include "CFBaseWeapon.h"
#include "GameFramework/Character.h"
#include "CFEquipFinishedAnimNotify.h"
#include "CFReloadFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

// Sets default values for this component's properties
UCFWeaponComponent::UCFWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UCFWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void UCFWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}


void UCFWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld())return;

	for (auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<ACFBaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon)continue;

		Weapon->OnClipEmpty.AddUObject(this, &UCFWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UCFWeaponComponent::AttachWeaponToSocket(ACFBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UCFWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("invalid"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->Zoom(false);
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	const auto CurrentWeaponData =
		WeaponData.FindByPredicate([&](const FWeaponData& Data) { //
		return Data.WeaponClass == CurrentWeapon->GetClass();//
			});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}


void UCFWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void UCFWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void UCFWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void UCFWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void UCFWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = FindNotifyByClass<UCFEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UCFWeaponComponent::OnEquipFinished);
	}


	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = FindNotifyByClass<UCFReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify) continue;
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UCFWeaponComponent::OnReloadFinished);
	}
}


void UCFWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh())  return;

	EquipAnimInProgress = false;
}

void UCFWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	ReloadAnimInProgress = false;
}

bool UCFWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UCFWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UCFWeaponComponent::CanReload() const
{
	return CurrentWeapon//
		&& !EquipAnimInProgress//
		&& !ReloadAnimInProgress//
		&& CurrentWeapon->CanReload();
}

void UCFWeaponComponent::Reload()
{
	ChangeClip();
}

void UCFWeaponComponent::OnEmptyClip(ACFBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon) return;

	if (CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for (const auto Weapon : Weapons)
		{
			if (Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

void UCFWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UCFWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UCFWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool UCFWeaponComponent::TryToAddAmmo(TSubclassOf<ACFBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}

bool UCFWeaponComponent::NeedAmmo(TSubclassOf<ACFBaseWeapon> WeaponType)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}

void UCFWeaponComponent::Zoom(bool Enabled)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Zoom(Enabled);
	}
}