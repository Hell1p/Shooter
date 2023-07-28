// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "MovementComponent/ShooterCharacterMovementComp.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Shooter/Shooter.h"
#include "Shooter/Components/CombatComponent.h"
#include "Shooter/Items/Weapons/BaseWeapon.h"

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

	ShooterCharacterMovement = Cast<UShooterCharacterMovementComp>(GetCharacterMovement());
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
}

void AShooterCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Head->SetLeaderPoseComponent(Arms);
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
	Crouch();
}

void AShooterCharacter::CrouchReleased()
{
	UnCrouch();
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

void AShooterCharacter::PlayFireMontage(bool bAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = Legs->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("") : FName("ARHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AShooterCharacter::AimOffset(float DeltaTime)
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// Map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}
