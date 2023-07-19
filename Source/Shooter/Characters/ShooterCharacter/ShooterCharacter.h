// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UShooterCharacterMovementComp;
class UCameraComponent;

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;
	
	/** Bindings */
	virtual void Jump() override;
	void MoveForward(float DeltaTime);
	void MoveRight(float DeltaTime);
	void LookUp(float DeltaTime);
	void Turn(float DeltaTime);
	void SprintPressed();
	void SprintReleased();
	void CrouchPressed();
	void CrouchReleased();
	
private:
	/** Movement */
	bool bSprinting;
	
	/** Components */
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Arms;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Legs;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere)
	UShooterCharacterMovementComp* ShooterCharacterMovement;
	
public:	
	FORCEINLINE bool GetbSprinting() const { return bSprinting; }
};
