// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GPRTurretProjectileBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Actors/GPREnemyBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AGPRTurretProjectileBase::AGPRTurretProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

// Called when the game starts or when spawned
void AGPRTurretProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Sets up function bindings
	SetupFunctionBindings();
}

// Called every frame
void AGPRTurretProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGPRTurretProjectileBase::SetupFunctionBindings()
{
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHitProjectileMesh);
}

void AGPRTurretProjectileBase::OnComponentHitProjectileMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Destroys this actor when it collides with another object
	this->Destroy();

	// Checks if the projectile hit an enemy object
	if (TObjectPtr<AGPREnemyBase> LocalHitEnemy = Cast<AGPREnemyBase>(OtherActor))
	{
		// Applies damage to the enemy target
		UGameplayStatics::ApplyDamage(LocalHitEnemy, ProjectileDamage,
		                              GetInstigatorController(), this,
		                              UDamageType::StaticClass());
	}

	// Null ptr check
	if (IsValid(ProjectileHitVFX))
	{
		// Spawns a niagara system at the location of impact
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileHitVFX,
													   Hit.ImpactPoint, FRotator::ZeroRotator,
													   FVector::OneVector, true,
													   true, ENCPoolMethod::None,
													   true);
	}

	// Null ptr check
	if (IsValid(ProjectileHitSFX))
	{
		// Plays a hit sound effect at the hit location
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileHitSFX, Hit.ImpactPoint);
	}
}

