// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shooter/ShooterStates.h"
#include "ShooterCharacter.generated.h"

class UCombatComponent;
class UShooterCharacterMovementComp;
class UCameraComponent;

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void PlayFireMontage(bool bAiming);

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
	void EquipButtonPressed();
	void FireButtonPressed();
	void FireButtonReleased();

private:
	UFUNCTION(Server, Reliable)
	void Server_EquipItem();
	void EquipItem();

	/** States */
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EWeaponType WeaponType = EWeaponType::EWT_None;

	/** Montages */
	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* FireWeaponMontage;
	
	/** Movement */
	void AimOffset(float DeltaTime);
	float AO_Yaw;
	float AO_Pitch;
	
	bool bSprinting;
	
	/** Components */
	UPROPERTY(VisibleAnywhere)
	UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Body;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Arms;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Legs;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponSwayScene;
	
	UPROPERTY(VisibleAnywhere)
	UShooterCharacterMovementComp* ShooterCharacterMovement;
	
public:	
	FORCEINLINE void SetCharacterState(ECharacterState State) { CharacterState = State; }
	
	FORCEINLINE bool GetbSprinting() const { return bSprinting; }
	FORCEINLINE USkeletalMeshComponent* GetArmsMesh() const { return Arms; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE UCameraComponent* GetCamera() const { return FollowCamera; }
};
