// Fill out your copyright notice in the Description page of Project Settings.


#include "CFBaseCharacter.h"
#include "CFCharacterMovementComponent.h"
#include "CFHealthComponent.h"
#include "Components/TextRenderComponent.h"
//#include "Engine/DamageEvents.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "CFWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

//ACFCharacter::ACFCharacter(){}

// Sets default values
ACFBaseCharacter::ACFBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass< UCFCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UCFHealthComponent>("HealthComponent");

	WeaponComponent = CreateDefaultSubobject<UCFWeaponComponent>("WeaponComponent");
	
}

// Called when the game starts or when spawned
void ACFBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(GetCharacterMovement());
	check(GetMesh());

	OnHealthChanged(HealthComponent->GetHealth(), 0.f);
	HealthComponent->OnDeath.AddUObject(this, &ACFBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ACFBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ACFBaseCharacter::OnGroundLanded);
	
}

// Called every frame
void ACFBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto Health = HealthComponent->GetHealth();
}


bool ACFBaseCharacter::IsRunning() const
{
	return false;
}


float ACFBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}



void ACFBaseCharacter::OnDeath()
{

	PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(LifeSpanOnDeath);//<-!

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ACFBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
	
}

void ACFBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	
	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	
		TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ACFBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if (!MaterialInst) return;

	MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

void ACFBaseCharacter::TurnOff()
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);
	
	Super::TurnOff();
}

void ACFBaseCharacter::Reset()
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);

	Super::Reset();
}