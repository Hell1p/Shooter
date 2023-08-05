// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCameraManager.h"
#include "Shooter/Characters/ShooterCharacter/ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Shooter/Characters/ShooterCharacter/MovementComponent/ShooterCharacterMovementComp.h"

AShooterCameraManager::AShooterCameraManager()
{
}

void AShooterCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UShooterCharacterMovementComp* ShooterMovementComponent = ShooterCharacter->GetShooterCharacterMovementComponent();
		if (ShooterMovementComponent == nullptr) return;

		FVector TargetCrouchOffset = FVector(
			0.f,
			0.f,
			ShooterMovementComponent->GetCrouchedHalfHeight() - ShooterCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);

		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (ShooterMovementComponent->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		if (ShooterMovementComponent->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
}
