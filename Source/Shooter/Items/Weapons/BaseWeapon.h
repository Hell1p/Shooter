// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shooter/Items/Item.h"
#include "BaseWeapon.generated.h"

class UAnimationAsset;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class SHOOTER_API ABaseWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	ABaseWeapon();
	virtual void Fire();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	float Damage = 5.f;
	
	UPROPERTY(BlueprintReadOnly)
	EWeaponState WeaponState = EWeaponState::EWS_Initial;
	
private:
	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	bool bAutomatic = true;
	
	/** Components */
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponMesh;
	
	/** Animations */
	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	UAnimationAsset* FireAnimation;
	
public:
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetFireDelay() const { return FireDelay; }
	FORCEINLINE bool GetbAutomatic() const { return bAutomatic; }
};
