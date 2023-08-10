// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class USoundCue;
class ABaseWeapon;

UCLASS()
class SHOOTER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	virtual void HandleOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit, ABaseWeapon* Weapon);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	UParticleSystemComponent* TracerComponent;

	EPhysicalSurface SurfaceType;

#pragma region ImpactEffects
	
	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	UParticleSystem* Rock_ImpactParticles;
	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	USoundCue* Rock_ImpactSound;

	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	UParticleSystem* Metal_ImpactParticles;
	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	USoundCue* Metal_ImpactSound;

	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	UParticleSystem* Flesh_ImpactParticles;
	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	USoundCue* Flesh_ImpactSound;

#pragma endregion ImpactEffects
};
