// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "MovementComponent/ShooterCharacterMovementComp.h"
#include "Camera/CameraComponent.h"

AShooterCharacter::AShooterCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UShooterCharacterMovementComp>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Arms->SetupAttachment(RootComponent);
	Arms->bOnlyOwnerSee = true;
	
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Arms);
	Head->bOwnerNoSee = true;
	Head->bOnlyOwnerSee = true;
	Head->bCastHiddenShadow = true;
	
	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(Arms);
	Legs->bOnlyOwnerSee = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(Arms);
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
}

void AShooterCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Head->SetLeaderPoseComponent(Arms);
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

