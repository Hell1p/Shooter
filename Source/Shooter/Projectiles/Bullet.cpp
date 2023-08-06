// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Shooter/Items/Weapons/BaseWeapon.h"
#include "Shooter/Characters/ShooterCharacter/ShooterCharacter.h"
#include "Shooter/Items/Weapons/ProjectileWeapon.h"
#include "Kismet/GameplayStatics.h"

void ABullet::HandleOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit, ABaseWeapon* Weapon)
{
	UGameplayStatics::ApplyDamage(OtherActor, Weapon->GetDamage(), nullptr, Weapon->GetOwner(), UDamageType::StaticClass());
}
