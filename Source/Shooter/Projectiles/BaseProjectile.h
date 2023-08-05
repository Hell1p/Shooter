// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class USoundCue;

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
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	UParticleSystemComponent* TracerComponent;

	EPhysicalSurface SurfaceType;
	
	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	UParticleSystem* Rock_ImpactParticles;
	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	USoundCue* Rock_ImpactSound;

	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	UParticleSystem* Metal_ImpactParticles;
	UPROPERTY(EditAnywhere, Category = ImpactEffects)
	USoundCue* Metal_ImpactSound;
};
