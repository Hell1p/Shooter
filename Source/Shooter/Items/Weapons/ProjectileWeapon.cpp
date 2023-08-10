// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Shooter/Projectiles/BaseProjectile.h"
#include "Shooter/Characters/ShooterCharacter/ShooterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/PlayerController.h"
#include "Shooter/Characters/ShooterCharacter/PlayerController/ShooterPlayerController.h"
#include "Camera/CameraComponent.h"

void AProjectileWeapon::Fire()
{
	if (ProjectileClass == nullptr) return;
	Super::Fire();

	const USkeletalMeshSocket* MuzzleSocket = GetWeaponMesh()->GetSocketByName(FName("S_Muzzle"));
	if (MuzzleSocket == nullptr) return;

	Character = Character == nullptr ? Cast<AShooterCharacter>(GetOwner()) : Character;
	if (Character == nullptr) return;

	FVector MuzzleLocation = MuzzleSocket->GetSocketLocation(GetWeaponMesh());

	FVector CameraLocation;
	FRotator CameraRotation;

	CameraRotation = Character->GetCamera()->GetComponentRotation();

	FVector CameraDirection = CameraRotation.Vector();
	FRotator BulletRotation = CameraDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	if (Character->GetNeutralizeTraceDist() > Character->GetDefaultWeaponNeutralizeTraceDist())
	{
		ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, BulletRotation, SpawnParams);
		if (Projectile) Projectile->SetOwner(this);
	}
	else
	{
		ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, GetActorForwardVector().Rotation(), SpawnParams);
		if (Projectile) Projectile->SetOwner(this);
	}
}
