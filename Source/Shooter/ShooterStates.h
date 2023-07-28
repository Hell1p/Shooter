#pragma once
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_Equipped UMETA(DisplayName = "Equipped"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_None UMETA(DisplayName = "None"),
	EWT_AR UMETA(DisplayName = "AR"),
	EWT_KA47 UMETA(DisplayName = "KA47"),

	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};