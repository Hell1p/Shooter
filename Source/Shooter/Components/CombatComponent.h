// Fill out your copyright notice in the Description page of Project Settings.ч

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AShooterCharacter;
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
};
