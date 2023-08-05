// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shooter/ShooterStates.h"
#include "ShooterCharacter.generated.h"

class UCombatComponent;
class UShooterCharacterMovementComp;
class UCameraComponent;
class AShooterPlayerController;
class APlayerController;

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
	
	void PlayFireMontage(bool bIsAiming);

protected:
	virtual void BeginPlay() override;
	
#pragma region Bindings

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
	void AimButtonPressed();
	void AimButtonReleased();

	void LeanLeftButtonPressed();
	void LeanLeftButtonReleased();
	UFUNCTION(Server, Reliable)
	void Server_LeanLeftButtonPressed(float bPressed);

	void LeanRightButtonPressed();
	void LeanRightButtonReleased();
	UFUNCTION(Server, Reliable)
	void Server_LeanRightButtonPressed(float bPressed);

#pragma endregion Bindings

private:
	UFUNCTION(Server, Reliable)
	void Server_EquipItem();
	void EquipItem();

	void RepControlRotation(float DeltaTime);

	/*UFUNCTION(Server, Reliable)
	void Server_ReplicatePitch(FRotator Rotation);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicatePitch(FRotator Rotation);*/

	UPROPERTY(Replicated)
	FRotator ControlRotation_Rep;

#pragma region PlayerProperties

	UPROPERTY(EditDefaultsOnly, Category = PlayerProperties)
	int MaxHealth;
	UPROPERTY(VisibleAnywhere, Replicated, Category = PlayerProperties)
	int CurrentHealth;

#pragma endregion PlayerProperties

#pragma region ADS

	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bAiming;
	float CurrentFOV;
	float DefaultFOV;
	UPROPERTY(EditAnywhere, Category = PlayerProperties)
	float AimFOV = 80.f;

	void HandleFOV(float DeltaTime);

#pragma endregion ADS

#pragma region States

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EWeaponType WeaponType = EWeaponType::EWT_None;
	UPROPERTY(Replicated)
	ECharacterLeanState CharacterLeanState = ECharacterLeanState::ECLS_None;

#pragma endregion States

#pragma region Montages

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* FireWeaponMontage;
	
#pragma endregion Montages

#pragma region Movement
	
	bool bSprinting;

#pragma endregion Movement

#pragma region Components
	
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
	USceneComponent* ADSScene;
	
	UPROPERTY(VisibleAnywhere)
	UShooterCharacterMovementComp* ShooterCharacterMovement;

#pragma endregion Components

public:	
	FORCEINLINE void SetCharacterState(ECharacterState State) { CharacterState = State; }
	FORCEINLINE void SetbAiming(bool Aiming) { bAiming = Aiming; }

	FORCEINLINE bool GetbSprinting() const { return bSprinting; }
	FORCEINLINE USkeletalMeshComponent* GetArmsMesh() const { return Arms; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE ECharacterLeanState GetLeanState() const { return CharacterLeanState; }
	FORCEINLINE UCameraComponent* GetCamera() const { return FollowCamera; }
	FORCEINLINE FRotator GetControlRotation_Rep() const { return ControlRotation_Rep; }
	FORCEINLINE UShooterCharacterMovementComp* GetShooterCharacterMovementComponent() const { return ShooterCharacterMovement; }
	FORCEINLINE bool GetbAiming() const { return bAiming; }
};
