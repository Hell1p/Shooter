// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacterMovementComp.h"
#include "GameFramework/Character.h"

UShooterCharacterMovementComp::UShooterCharacterMovementComp()
{
	
}

UShooterCharacterMovementComp::FNetworkPredictionData_Client_Shooter::FNetworkPredictionData_Client_Shooter(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{
}

void UShooterCharacterMovementComp::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UShooterCharacterMovementComp::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

FNetworkPredictionData_Client* UShooterCharacterMovementComp::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UShooterCharacterMovementComp* MutableThis = const_cast<UShooterCharacterMovementComp*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Shooter(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

void UShooterCharacterMovementComp::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UShooterCharacterMovementComp::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

bool UShooterCharacterMovementComp::FSavedMove_Shooter::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                       ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Shooter* NewShooterMove = static_cast<FSavedMove_Shooter*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewShooterMove->Saved_bWantsToSprint)
	{
		return false;
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UShooterCharacterMovementComp::FSavedMove_Shooter::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UShooterCharacterMovementComp::FSavedMove_Shooter::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	
	if (Saved_bWantsToSprint) Result |= FLAG_Custom_0;
	
	return Result;
}

void UShooterCharacterMovementComp::FSavedMove_Shooter::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UShooterCharacterMovementComp* CharacterMovement = Cast<UShooterCharacterMovementComp>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UShooterCharacterMovementComp::FSavedMove_Shooter::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UShooterCharacterMovementComp* CharacterMovement = Cast<UShooterCharacterMovementComp>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

FSavedMovePtr UShooterCharacterMovementComp::FNetworkPredictionData_Client_Shooter::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Shooter());
}
