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
	
	UPROPERTY(BlueprintReadOnly)
	EWeaponState WeaponState = EWeaponState::EWS_Initial;
	
private:
#pragma region WeaponProperties

	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	float Damage = 5.f;

	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	bool bAutomatic = true;

#pragma endregion WeaponProperties

#pragma region Components

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponMesh;
	
#pragma endregion Components

#pragma region Animations

	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	UAnimationAsset* FireAnimation;

#pragma endregion Animations
	
public:
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetFireDelay() const { return FireDelay; }
	FORCEINLINE bool GetbAutomatic() const { return bAutomatic; }
	FORCEINLINE float GetDamage() const { return Damage; }
};
