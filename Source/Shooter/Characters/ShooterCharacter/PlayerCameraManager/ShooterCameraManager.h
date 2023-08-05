// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ShooterCameraManager.generated.h"

UCLASS()
class SHOOTER_API AShooterCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	AShooterCameraManager();
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration = .15f;

	float CrouchBlendTime;
};
