// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "ProjectileWeapon.generated.h"

class ABaseProjectile;
class UCameraComponent;
class AShooterCharacter;

UCLASS()
class SHOOTER_API AProjectileWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire() override;

private:
	AShooterCharacter* Character;
	
	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	TSubclassOf<ABaseProjectile> ProjectileClass;

};
