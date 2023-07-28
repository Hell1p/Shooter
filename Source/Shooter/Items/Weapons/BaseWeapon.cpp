// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Animation/AnimationAsset.h"
#include "Shooter/Shooter.h"
#include "Components/SkeletalMeshComponent.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCollisionObjectType(ECC_Item);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Item, ECollisionResponse::ECR_Ignore);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::Fire()
{
	if (FireAnimation == nullptr) return;
	WeaponMesh->PlayAnimation(FireAnimation, false);
}
