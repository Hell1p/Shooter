// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "Bullet.generated.h"

UCLASS()
class SHOOTER_API ABullet : public ABaseProjectile
{
	GENERATED_BODY()
	
protected:
	virtual void HandleOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit, ABaseWeapon* Weapon) override;
};
