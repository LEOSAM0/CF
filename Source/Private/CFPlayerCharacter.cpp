// Fill out your copyright notice in the Description page of Project Settings.


#include "CFPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CFWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ACFPlayerCharacter::ACFPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.f, 100.f, 80.f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ACFPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACFPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACFPlayerCharacter::OnCameraCollisionEndOverlap);

}

void ACFPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void ACFPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ACFPlayerCharacter::CheckCameraOverlap()
{
	const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);

	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (auto MeshChild : MeshChildren)
	{
		const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
		if (MeshChildGeometry)
		{
			MeshChildGeometry->SetOwnerNoSee(HideMesh);
		}
	}
}

// Called to bind functionality to input
void ACFPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACFPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACFPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACFPlayerCharacter::LookingUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &ACFPlayerCharacter::LookingAround);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACFPlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACFPlayerCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACFPlayerCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UCFWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UCFWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UCFWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UCFWeaponComponent::Reload);

	DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
	PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, WeaponComponent, &UCFWeaponComponent::Zoom, true);
	PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, WeaponComponent, &UCFWeaponComponent::Zoom, false);

	//PlayerInputComponent->BindAction("Action", IE_Pressed, WeaponComponent, &UCFWeaponComponent::Reload);
}

bool ACFPlayerCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void ACFPlayerCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.f;
	if (Amount == 0) return;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ACFPlayerCharacter::MoveRight(float Amount)
{
	if (Amount == 0) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ACFPlayerCharacter::LookingUp(float Value)
{
	AddControllerPitchInput(Value * LookingUpAround);
}

void ACFPlayerCharacter::LookingAround(float Value)
{
	AddControllerYawInput(Value * LookingUpAround);
}

void ACFPlayerCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ACFPlayerCharacter::OnStopRunning()
{
	WantsToRun = false;
}


void ACFPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	
}
