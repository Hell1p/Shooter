// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Shooter/Items/Weapons/BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Shooter/Characters/ShooterCharacter/ShooterCharacter.h"
#include "TimerManager.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	const USkeletalMeshSocket* WeaponSocket = Character->GetArmsMesh()->GetSocketByName(FName("S_Weapon"));
	if (WeaponSocket)
	{
		EquippedWeapon->DisableComponentsSimulatePhysics();
		WeaponSocket->AttachActor(EquippedWeapon, Character->GetArmsMesh());
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		Character->SetCharacterState(ECharacterState::ECS_Equipped);
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	if (EquippedWeapon == nullptr) return;
	bFireButtonPressed = bPressed;

	if (bFireButtonPressed) Fire();
}

void UCombatComponent::Server_Fire_Implementation()
{
	Multicast_Fire();
}

void UCombatComponent::Multicast_Fire_Implementation()
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	
	Character->PlayFireMontage(false);
	EquippedWeapon->Fire();
}

void UCombatComponent::StartFireTimer()
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;

	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UCombatComponent::FireTimerFinished, EquippedWeapon->GetFireDelay());
}

void UCombatComponent::FireTimerFinished()
{
	bCanFire = true;
	if (EquippedWeapon == nullptr) return;
	
	if (bFireButtonPressed && EquippedWeapon->GetbAutomatic())
	{
		Fire();
	}
}

void UCombatComponent::Fire()
{
	if (bCanFire)
	{
		bCanFire = false;
		Server_Fire();
		StartFireTimer();
	}
}

void UCombatComponent::AimButtonPressed(bool bPressed)
{
	if (bPressed)
	{
		Server_Aim(true);
	}
	else
	{
		Server_Aim(false);
	}
}

void UCombatComponent::Server_Aim_Implementation(bool bPressed)
{
	if (Character == nullptr) return;

	if (bPressed) Character->SetbAiming(true);
	else Character->SetbAiming(false);
}
