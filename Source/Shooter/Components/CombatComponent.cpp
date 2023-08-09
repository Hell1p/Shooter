// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Shooter/Items/Weapons/BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Shooter/Characters/ShooterCharacter/ShooterCharacter.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TimelineComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Server_HandleRecoil(DeltaTime);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, YawInput);
	DOREPLIFETIME(UCombatComponent, PitchInput);
	DOREPLIFETIME(UCombatComponent, StartRotation);
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
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	bFireButtonPressed = bPressed;
	StartRecoil();

	if (bFireButtonPressed)
	{
		Fire();
		StartRotation = Character->GetControlRotation();
	}
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

	if (!bFireButtonPressed) EndRecoil();
	
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

#pragma region Recoil

void UCombatComponent::StartHorizontalRecoil(float Value)
{
	if (Character == nullptr || RecoilTimeline.IsReversing()) return;
	Character->AddControllerYawInput(Value);
}

void UCombatComponent::StartVerticalRecoil(float Value)
{
	if (Character == nullptr || RecoilTimeline.IsReversing()) return;
	Character->AddControllerPitchInput(Value);
}

void UCombatComponent::StartRecoil()
{
	Server_StartRecoil();
}

void UCombatComponent::Server_StartRecoil_Implementation()
{
	Multicast_StartRecoil();
}

void UCombatComponent::Multicast_StartRecoil_Implementation()
{
	RecoilTimeline.PlayFromStart();
}

void UCombatComponent::EndRecoil()
{
	Server_EndRecoil();
}

void UCombatComponent::Server_EndRecoil_Implementation()
{
	Multicast_EndRecoil();
}

void UCombatComponent::Multicast_EndRecoil_Implementation()
{
	RecoilTimeline.ReverseFromEnd();
}

void UCombatComponent::SetupRecoil()
{
	Multicast_SetupRecoil();
}

void UCombatComponent::Server_SetupRecoil_Implementation()
{
	Multicast_SetupRecoil();
}

void UCombatComponent::Multicast_SetupRecoil_Implementation()
{
	FOnTimelineFloat XRecoilCurve;
	FOnTimelineFloat YRecoilCurve;

	XRecoilCurve.BindUFunction(this, FName("StartHorizontalRecoil"));
	YRecoilCurve.BindUFunction(this, FName("StartVerticalRecoil"));

	if (HorizontalCurve == nullptr || VerticalCurve == nullptr) return;

	RecoilTimeline.AddInterpFloat(HorizontalCurve, XRecoilCurve);
	RecoilTimeline.AddInterpFloat(VerticalCurve, YRecoilCurve);
}

void UCombatComponent::HandleRecoil(float DeltaTime)
{
	Server_HandleRecoil(DeltaTime);
}

void UCombatComponent::Server_HandleRecoil_Implementation(float DeltaTime)
{
	Multicast_HandleRecoil(DeltaTime);
}

void UCombatComponent::Multicast_HandleRecoil_Implementation(float DeltaTime)
{
	if (RecoilTimeline.IsPlaying())
	{
		RecoilTimeline.TickTimeline(DeltaTime);
	}

	if (RecoilTimeline.IsReversing())
	{
		if (Character == nullptr) return;
		if (FMath::Abs(YawInput) > 0.005f || FMath::Abs(PitchInput) > 0.005f)
		{
			RecoilTimeline.Stop();
			return;
		}

		RecoilTimeline.TickTimeline(DeltaTime);
		FRotator NewRotation = UKismetMathLibrary::RInterpTo(Character->GetControlRotation(), StartRotation, DeltaTime, 5.f);

		if (Character->Controller) Character->Controller->ClientSetRotation(NewRotation);
	}
}

#pragma endregion Recoil
