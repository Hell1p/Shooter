// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacterAnimInstance.h"

#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Shooter/Characters/ShooterCharacter/ShooterCharacter.h"

void UShooterCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UShooterCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	ShooterCharacter == nullptr ? Cast<AShooterCharacter>(TryGetPawnOwner()) : ShooterCharacter;
	if (ShooterCharacter == nullptr) return;
	
	Speed = ShooterCharacter->GetVelocity().Size();
	bIsCrouching = ShooterCharacter->bIsCrouched;
	bIsSprinting = ShooterCharacter->GetbSprinting();
	Direction = UKismetAnimationLibrary::CalculateDirection(ShooterCharacter->GetVelocity(), ShooterCharacter->GetActorRotation());
	bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();
	CharacterState = ShooterCharacter->GetCharacterState();
	WeaponType = ShooterCharacter->GetWeaponType();
	AO_Pitch = ShooterCharacter->GetAO_Pitch();
}