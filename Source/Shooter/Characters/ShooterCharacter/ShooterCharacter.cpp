// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "MovementComponent/ShooterCharacterMovementComp.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Shooter/Shooter.h"
#include "Shooter/Components/CombatComponent.h"
#include "Shooter/Items/Weapons/BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Shooter/Characters/ShooterCharacter/PlayerController/ShooterPlayerController.h"

AShooterCharacter::AShooterCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UShooterCharacterMovementComp>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Body->SetHiddenInGame(true);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(Body, FName("Camera"));

	WeaponSwayScene = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSwayScene"));
	WeaponSwayScene->SetupAttachment(FollowCamera);

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->SetupAttachment(WeaponSwayScene);
	Arms->bOnlyOwnerSee = true;
	
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Arms);
	Head->bOwnerNoSee = true;
	Head->bOnlyOwnerSee = true;
	Head->bCastHiddenShadow = true;
	 
	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(RootComponent);
	Legs->bOnlyOwnerSee = true;	
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AShooterCharacter::ReceiveDamage);

	DefaultFOV = FollowCamera->FieldOfView;
	CurrentHealth = MaxHealth;

	ShooterCharacterMovement = Cast<UShooterCharacterMovementComp>(GetCharacterMovement());
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RepControlRotation(DeltaTime);
	HandleFOV(DeltaTime);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AShooterCharacter::Turn);

	PlayerInputComponent->BindAction(FName("Sprint"), IE_Pressed, this, &AShooterCharacter::SprintPressed);
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Released, this, &AShooterCharacter::SprintReleased);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Crouch"), IE_Pressed, this, &AShooterCharacter::CrouchPressed);
	PlayerInputComponent->BindAction(FName("Crouch"), IE_Released, this, &AShooterCharacter::CrouchReleased);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AShooterCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Pressed, this, &AShooterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Released, this, &AShooterCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction(FName("Aim"), IE_Pressed, this, &AShooterCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction(FName("Aim"), IE_Released, this, &AShooterCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction(FName("LeanLeft"), IE_Pressed, this, &AShooterCharacter::LeanLeftButtonPressed);
	PlayerInputComponent->BindAction(FName("LeanLeft"), IE_Released, this, &AShooterCharacter::LeanLeftButtonReleased);
	PlayerInputComponent->BindAction(FName("LeanRight"), IE_Pressed, this, &AShooterCharacter::LeanRightButtonPressed);
	PlayerInputComponent->BindAction(FName("LeanRight"), IE_Released, this, &AShooterCharacter::LeanRightButtonReleased);
}

void AShooterCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Head->SetLeaderPoseComponent(Arms);
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterCharacter, ControlRotation_Rep, COND_SkipOwner);
	DOREPLIFETIME(AShooterCharacter, CurrentHealth);
	DOREPLIFETIME(AShooterCharacter, bAiming);
	DOREPLIFETIME(AShooterCharacter, CharacterLeanState);
}

void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat) Combat->Character = this;
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Controller && (Value != 0))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f,ControlRotation.Yaw,0.f);
		const FVector Direction  = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Controller && (Value != 0))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,Value);
	}
}

void AShooterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AShooterCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AShooterCharacter::SprintPressed()
{
	ShooterCharacterMovement == nullptr ? ShooterCharacterMovement = Cast<UShooterCharacterMovementComp>(GetCharacterMovement()) : ShooterCharacterMovement;
	
	ShooterCharacterMovement->SprintPressed();
	bSprinting = true;
}

void AShooterCharacter::SprintReleased()
{
	ShooterCharacterMovement == nullptr ? ShooterCharacterMovement = Cast<UShooterCharacterMovementComp>(GetCharacterMovement()) : ShooterCharacterMovement;	
	
	ShooterCharacterMovement->SprintReleased();
	bSprinting = false;
}

void AShooterCharacter::CrouchPressed()
{
	ShooterCharacterMovement == nullptr ? ShooterCharacterMovement = Cast<UShooterCharacterMovementComp>(GetCharacterMovement()) : ShooterCharacterMovement;

	ShooterCharacterMovement->CrouchPressed();
}

void AShooterCharacter::CrouchReleased()
{
	ShooterCharacterMovement == nullptr ? ShooterCharacterMovement = Cast<UShooterCharacterMovementComp>(GetCharacterMovement()) : ShooterCharacterMovement;

	ShooterCharacterMovement->CrouchReleased();
}

void AShooterCharacter::Jump()
{
	Super::Jump();
}

void AShooterCharacter::EquipButtonPressed()
{
	if (HasAuthority()) EquipItem();
	else Server_EquipItem();
}

void AShooterCharacter::Server_EquipItem_Implementation()
{
	EquipItem();
}

void AShooterCharacter::EquipItem()
{
	if (Combat == nullptr) return;
	
	FHitResult HitResult;
	FVector Start = FollowCamera->GetComponentLocation();
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, GetController()->GetControlRotation().Vector() * 1500.f + Start, ECC_Item);

	ABaseWeapon* Weapon = Cast<ABaseWeapon>(HitResult.GetActor());
	if (Weapon == nullptr) return;
	
	Combat->EquippedWeapon = Weapon;
	Combat->EquippedWeapon->Owner = this;
	Combat->EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* WeaponSocket = Arms->GetSocketByName(FName("S_Weapon"));
	if (WeaponSocket)
	{
		Weapon->DisableComponentsSimulatePhysics();
		WeaponSocket->AttachActor(Weapon, Arms);
	}
	CharacterState = ECharacterState::ECS_Equipped;
}

void AShooterCharacter::FireButtonPressed()
{
	if (Combat) Combat->FireButtonPressed(true);
}

void AShooterCharacter::FireButtonReleased()
{
	if (Combat) Combat->FireButtonPressed(false);
}

void AShooterCharacter::PlayFireMontage(bool IsbAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = Legs->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = IsbAiming ? FName("") : FName("ARHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AShooterCharacter::RepControlRotation(float DeltaTime)
{
	if (HasAuthority() || IsLocallyControlled())
	{
		ControlRotation_Rep = GetController()->GetControlRotation();
	}
}

void AShooterCharacter::HandleFOV(float DeltaTime)
{
	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, AimFOV, DeltaTime, 10.f);
	}
	else if (CurrentFOV != DefaultFOV)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, 10.f);
	}
	FollowCamera->SetFieldOfView(CurrentFOV);
}

void AShooterCharacter::AimButtonPressed()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	Combat->AimButtonPressed(true);
}

void AShooterCharacter::AimButtonReleased()
{
	if (Combat == nullptr) return;
	Combat->AimButtonPressed(false);
}

void AShooterCharacter::LeanLeftButtonPressed()
{
	Server_LeanLeftButtonPressed(true);	
}

void AShooterCharacter::LeanLeftButtonReleased()
{
	Server_LeanLeftButtonPressed(false);
}

void AShooterCharacter::LeanRightButtonPressed()
{
	Server_LeanRightButtonPressed(true);
}

void AShooterCharacter::LeanRightButtonReleased()
{
	Server_LeanRightButtonPressed(false);
}

void AShooterCharacter::Server_LeanLeftButtonPressed_Implementation(float bPressed)
{
	if (bPressed)
	{
		CharacterLeanState = ECharacterLeanState::ECLS_Left;
	}
	else
	{
		if (!(CharacterLeanState == ECharacterLeanState::ECLS_Right)) CharacterLeanState = ECharacterLeanState::ECLS_None;
	}
}

void AShooterCharacter::Server_LeanRightButtonPressed_Implementation(float bPressed)
{
	if (bPressed)
	{
		CharacterLeanState = ECharacterLeanState::ECLS_Right;
	}
	else
	{
		if (!(CharacterLeanState == ECharacterLeanState::ECLS_Left)) CharacterLeanState = ECharacterLeanState::ECLS_None;
	}
}

void AShooterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);

	if (CurrentHealth == 0.f) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Dead"));
}