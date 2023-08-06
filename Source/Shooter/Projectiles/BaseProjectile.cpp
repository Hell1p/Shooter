// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "Shooter/Shooter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Shooter/Items/Weapons/BaseWeapon.h"
#include "Shooter/Characters/ShooterCharacter/ShooterCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CollisionBox->bReturnMaterialOnMove = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 15000.f;
	ProjectileMovementComponent->MaxSpeed = 15000.f;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBox, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
	}

	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	}
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseProjectile::Destroyed()
{
	Super::Destroyed();

	if (SurfaceType == NULL) return;
	if (SurfaceType == EPS_Stone)
	{
		if (Rock_ImpactParticles) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Rock_ImpactParticles, GetActorTransform());
		if (Rock_ImpactSound) UGameplayStatics::PlaySoundAtLocation(this, Rock_ImpactSound, GetActorLocation());
	}
	else if (SurfaceType == EPS_Metal)
	{
		if (Metal_ImpactParticles) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Metal_ImpactParticles, GetActorTransform());
		if (Metal_ImpactSound) UGameplayStatics::PlaySoundAtLocation(this, Metal_ImpactSound, GetActorLocation());
	}
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABaseWeapon* Weapon = Cast<ABaseWeapon>(GetOwner());
	if (Weapon == nullptr) return;
	AShooterCharacter* OwnerCharacter = Cast<AShooterCharacter>(Weapon->GetOwner());
	if (OwnerCharacter == nullptr) return;

	UPhysicalMaterial* PhysMaterial = Hit.PhysMaterial.Get();
	SurfaceType = PhysMaterial->SurfaceType;
	if (OwnerCharacter->HasAuthority())
	{
		if (OtherComp->IsSimulatingPhysics()) OtherComp->AddImpulseAtLocation(GetVelocity() - 100.f, GetActorLocation());

		HandleOnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit, Weapon);
	}

	Destroy();
}

void ABaseProjectile::HandleOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit, ABaseWeapon* Weapon)
{

}
