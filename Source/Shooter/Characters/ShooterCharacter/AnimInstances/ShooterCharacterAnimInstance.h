// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Shooter/ShooterStates.h"
#include "ShooterCharacterAnimInstance.generated.h"

class AShooterCharacter;

UCLASS()
class SHOOTER_API UShooterCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	AShooterCharacter* ShooterCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category = States, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = States, meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;
	
	UPROPERTY(BlueprintReadOnly, Category = States, meta = (AllowPrivateAccess = "true"))
	ECharacterLeanState CharacterLeanState;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Direction;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LeanLeft;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LeanRight;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FRotator ControlRotation_Rep;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;
};
