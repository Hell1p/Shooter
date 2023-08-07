// Fill out your copyright notice in the Description page of Project Settings.ч

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AShooterCharacter;
class UCurveFloat;
class ABaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend AShooterCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_Fire();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire();
	
private:
	UFUNCTION()
	void OnRep_EquippedWeapon();
	void FireButtonPressed(bool bPressed);
	void Fire();

	void AimButtonPressed(bool bPressed);
	UFUNCTION(Server, Reliable)
	void Server_Aim(bool bPressed);

	FTimerHandle FireTimer;
	
	bool bCanFire = true;
	
	void FireTimerFinished();
	void StartFireTimer();

	bool bFireButtonPressed;

	AShooterCharacter* Character;
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	ABaseWeapon* EquippedWeapon;

#pragma region Recoil

	void HandleRecoil(float DeltaTime);
	UFUNCTION(Server, Reliable)
	void Server_HandleRecoil(float DeltaTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HandleRecoil(float DeltaTime);

	UFUNCTION()
	void StartHorizontalRecoil(float Value);

	UFUNCTION()
	void StartVerticalRecoil(float Value);

	void StartRecoil();
	UFUNCTION(Server, Reliable)
	void Server_StartRecoil();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartRecoil();

	void EndRecoil();
	UFUNCTION(NetMulticast, Reliable)
	void Server_EndRecoil();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EndRecoil();

	void SetupRecoil();

	UFUNCTION(Server, Reliable)
	void Server_SetupRecoil();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetupRecoil();

	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	UCurveFloat* VerticalCurve;

	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	UCurveFloat* HorizontalCurve;

	//UPROPERTY(Replicated)
	FTimeline RecoilTimeline;

	UPROPERTY(Replicated)
	float YawInput;
	UPROPERTY(Replicated)
	float PitchInput;
	UPROPERTY(Replicated)
	FRotator StartRotation;

#pragma endregion Recoil
};
