// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GPRTurretBase.h"
#include "Actors/GPREnemyBase.h"
#include "Actors/GPRTurretProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Widgets/Turret/GPRTurretLevel.h"

// Sets default values
AGPRTurretBase::AGPRTurretBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	RootComponent = SceneRootComponent;
	
	TurretBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretBaseMesh");
	TurretBaseMesh->SetupAttachment(SceneRootComponent);

	TurretBodyRotationPoint = CreateDefaultSubobject<USceneComponent>("TurretBodyRotationPoint");
	TurretBodyRotationPoint->SetupAttachment(TurretBaseMesh);

	TurretBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretBodyMesh");
	TurretBodyMesh->SetupAttachment(TurretBodyRotationPoint);
	
	TurretHeadRotationPoint = CreateDefaultSubobject<USceneComponent>("HeadRotationPoint");
	TurretHeadRotationPoint->SetupAttachment(TurretBodyRotationPoint);
	
	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretHeadMesh");
	TurretHeadMesh->SetupAttachment(TurretHeadRotationPoint);
	
	TurretMuzzleComponent = CreateDefaultSubobject<USceneComponent>("TurretMuzzleComponent");
	TurretMuzzleComponent->SetupAttachment(TurretHeadMesh);

	TurretSphereComponent = CreateDefaultSubobject<USphereComponent>("TurretSphereComponent");
	TurretSphereComponent->SetupAttachment(SceneRootComponent);

	WidgetTurretLevelComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetTurretLevelComponent");
	WidgetTurretLevelComponent->SetupAttachment(SceneRootComponent);
}

// Called when the game starts or when spawned
void AGPRTurretBase::BeginPlay()
{
	Super::BeginPlay();

	// Binds a function to the sphere components begin overlap event
	TurretSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);

	// Checks if the widget level component is using the turret level widget
	if (TObjectPtr<UGPRTurretLevel> LocalTurretLevelWidgetRef = Cast<UGPRTurretLevel>(WidgetTurretLevelComponent->GetWidget()))
	{
		TurretLevelWidgetRef = LocalTurretLevelWidgetRef;
	}

	// Sets up the default projectiles speed
	SetupDefaultTurretProjectileSpeed();

	// Null ptr check
	if (IsValid(TurretSpawnSFX))
	{
		// Plays a sound effect when the turret spawns into the world
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TurretSpawnSFX, GetActorLocation());
	}
}

// Called every frame
void AGPRTurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FindEnemyTarget(); // Finds the turrets next enemy target to destroy
	ActivateTurretSystems(); // Activates the turrets systems when there is a valid enemy target
}

void AGPRTurretBase::Fire()
{
	// Spawn Parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the turret projectile (may need a reference to the spawned projectile later on)
	if (TObjectPtr<AGPRTurretProjectileBase> LocalTurretProjectile = GetWorld()->SpawnActor<AGPRTurretProjectileBase>(TurretProjectileClass,
								   TurretMuzzleComponent->GetComponentLocation(),
								   TurretMuzzleComponent->GetComponentRotation(),
								   SpawnParams))
	{
		// Sets the spawned projectiles damage to equal the turrets current damage based on it's current level
		LocalTurretProjectile->ProjectileDamage = CurrentTurretDamage;
	}

	// Plays niagara effect & sound effect when the turret fires
	PlayTurretShootingVFX();
	PlayTurretShootingSFX();
}

void AGPRTurretBase::SetYaw(float TargetYaw) const
{
	// Gets the turrets current rotation yaw value & saves it as a new local rotator
	float CurrentYaw = TurretBodyRotationPoint->GetRelativeRotation().Yaw;

	// Creates a new yaw variable which interps from the turrets current yaw rotation to the new yaw rotation
	float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, GetWorld()->GetDeltaSeconds(), TurnSpeed);

	// Sets the turrets yaw rotation based on the new interpolated yaw variable
	TurretBodyRotationPoint->SetRelativeRotation(FRotator(0.f, NewYaw, 0.f));
}

void AGPRTurretBase::SetPitch(float TargetPitch) const
{
	// Gets the turrets current rotation pitch value & saves it as a new local rotator
	float CurrentPitch = TurretHeadRotationPoint->GetRelativeRotation().Pitch;

	// Creates a new yaw variable which interps from the turrets current pitch rotation to the new pitch rotation
	float NewPitch = FMath::FInterpTo(CurrentPitch, TargetPitch, GetWorld()->GetDeltaSeconds(), TurnSpeed);
	
	// Sets the turrets yaw rotation based on the new interpolated pitch variable
	TurretHeadRotationPoint->SetRelativeRotation(FRotator(NewPitch, 0.f, 0.f));
}

void AGPRTurretBase::PlayTurretShootingSFX()
{
	// Null ptr check
	if (IsValid(TurretShootingSFX))
	{
		FVector LocalSoundPos = TurretMuzzleComponent->GetComponentLocation();

		// Plays a shooting sound at the turrets muzzle location
		UGameplayStatics::PlaySoundAtLocation(this, TurretShootingSFX, LocalSoundPos);
	}
}

void AGPRTurretBase::PlayTurretShootingVFX()
{
	// Null ptr check
	if (IsValid(TurretShootingVFX))
	{
		// Declares local vector & rotator values for cleaner code
		FVector LocalPos = TurretMuzzleComponent->GetComponentLocation();
		FRotator LocalRot = TurretMuzzleComponent->GetComponentRotation();
		LocalRot.Yaw -= 90.f;
		
		// Spawns in a niagara effect muzzle flash from the turrets muzzle location
		if (TObjectPtr<UNiagaraComponent> LocalNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), TurretShootingVFX, LocalPos, LocalRot))
		{
			// Declares a local timer handle to be used for the lambda timer
			FTimerHandle LocalTimerHandle;

			// Sets a timer to destroy the niagara effect after 1 second
			GetWorld()->GetTimerManager().SetTimer(LocalTimerHandle, [LocalNiagaraComp]()
			{
				// Null ptr check
				if (IsValid(LocalNiagaraComp))
				{
					// Destroys the niagara effect
					LocalNiagaraComp->DestroyComponent();
				}
			}, 0.1f, false);
		}
	}
}

void AGPRTurretBase::ResetTurretShooting()
{
	// Stops the turret from shooting
	GetWorld()->GetTimerManager().ClearTimer(TurretFireTimerHandle);

	// The turret is not currently shooting
	bIsShooting = false;
}

void AGPRTurretBase::FindEnemyTarget()
{
	// If the array is not empty than the turret will target the enemy who is at the top of the array list (1st element)
	if (!EnemyTargetsArray.IsEmpty())
	{
		// Checks if the target enemy is not valid
		if (!IsValid(TargetEnemy))
		{
			/* If the target enemy is not valid, a for each loop will
			 * loop through the array of targets */
			for (auto Enemy : EnemyTargetsArray)
			{
				// If the currently iterated enemy is valid, the enemy will now be the new target
				if (IsValid(Enemy))
				{
					TargetEnemy = Enemy;
					break;
				}
			}
		}

		// Null ptr check
		if (IsValid(TargetEnemy))
		{
			/* Checks if the enemy "OnDestroyed" event is already bound to the "OnEnemyDestroyed" function
			 * If it is not bound than it will bind the event to the function */
			if (!TargetEnemy->OnDestroyed.IsAlreadyBound(this, &ThisClass::AGPRTurretBase::OnEnemyDestroyed))
			{
				// Binds the "OnDestroyed" event to a function in this class
				TargetEnemy->OnDestroyed.AddDynamic(this, &ThisClass::OnEnemyDestroyed);
			}
		}
	}
}

void AGPRTurretBase::SetupDefaultTurretProjectileSpeed()
{
	/* Creates a reference to the default class of the turret projectile actor
	 * & checks if both the projectile & it's projectile movement component are valid */		
	if (const TObjectPtr<AGPRTurretProjectileBase> DefaultProjectile = Cast<AGPRTurretProjectileBase>(TurretProjectileClass->GetDefaultObject()))
	{
		// Creates a float var which stores the default value of the turret projectiles initial movement speed
		ProjectileSpeed = DefaultProjectile->ProjectileMovementComponent->InitialSpeed;
	}	
}

void AGPRTurretBase::ActivateTurretSystems()
{
	// Only activates the turret systems when there is a valid object to shoot
	if (IsValid(TargetEnemy))
	{
		// Calculates the time it will take for the turrets projectile to hit the target projectile
		TimeToImpact = TimeToImpactCalculation(TargetEnemy, TurretMuzzleComponent, ProjectileSpeed);

		// Calculates the position the turret needs to shoot at in order to hit the target projectile
		ImpactPointPos = ImpactPosCalculation(TargetEnemy);

		// Calculates the yaw & pitch rotation the turret must use to aim at the target
		TurretYawCalculation();
		TurretPitchCalculation();

		// Shoots the target when the target is within the turrets tolerance angle & if the turret is not already being shot
		if (IsTurretAimingAtTarget() && !bIsShooting)
		{
			// Sets a timer to fire the turrets projectiles at the target
			GetWorld()->GetTimerManager().SetTimer(TurretFireTimerHandle, this, &ThisClass::Fire, CurrentTurretFireRate, true, 0.f);
			
			// The turret is now shooting, this will stop the timer from being called every frame which will cause issues
			bIsShooting = true;
		}
	}
}

float AGPRTurretBase::TimeToImpactCalculation(const AActor* ActorToTarget, const USceneComponent* TurretMuzzle, const float TurretProjectileSpeed)
{
	// Stores the target actors velocity for easier readability
	FVector TargetActorVelocity = ActorToTarget->GetVelocity();

	// Adds some height to the actors location
	FVector TargetEnemyLocation = FVector(ActorToTarget->GetActorLocation().X, ActorToTarget->GetActorLocation().Y, ActorToTarget->GetActorLocation().Z + 20.f);
	
	// Gets the vector direction from the turrets muzzle pos to the target projectile
	FVector TargetDirection = TargetEnemyLocation - TurretMuzzle->GetComponentLocation();

	// Gets the squared magnitude of the target projectile & takes away the turrets squared projectile speed
	float LocalSpeedDifference = CustomVectorMagnitudeSquared(TargetActorVelocity) - FMath::Square(TurretProjectileSpeed);

	// How fast the target projectile is moving towards or away relative from the turret projectile
	float LocalRelativeMotion = CustomDotProductCalculation(TargetDirection, TargetActorVelocity) * 2.f;
	
	// Calculates the squared distance (magnitude) between the turret & the target
	float LocalInitialDistance = CustomVectorMagnitudeSquared(TargetDirection);

	// Determines if this calculation has a valid solution
	float LocalDiscriminant = FMath::Square(LocalRelativeMotion) - LocalSpeedDifference * 4.f * LocalInitialDistance;

	// Returns the final calculated time to impact
	return (LocalRelativeMotion * -1.f - FMath::Sqrt(FMath::Max(0.f, LocalDiscriminant))) / (LocalSpeedDifference * 2.f);
}

FVector AGPRTurretBase::ImpactPosCalculation(const AActor* ActorToTarget) const
{
	// Adds some height to the actors location
	FVector TargetEnemyLocation = FVector(ActorToTarget->GetActorLocation().X, ActorToTarget->GetActorLocation().Y, ActorToTarget->GetActorLocation().Z + 20.f);
	
	return TargetEnemyLocation + ActorToTarget->GetVelocity() * TimeToImpact;
}

void AGPRTurretBase::TurretYawCalculation()
{
	// Creates a local version of the calculated impact pos & then sets the world Z of that vector to be inline with the turret
	FVector LocalImpactPointPos = FVector(ImpactPointPos.X, ImpactPointPos.Y, GetActorLocation().Z);
	
	/* Gets the direction from the turrets muzzle to the target as a unit vector
	 * & the normalizes the direction to a unit vector */
	FVector DirectionUnitVector = LocalImpactPointPos - TurretMuzzleComponent->GetComponentLocation();
	DirectionUnitVector.Normalize(0.0001f);

	// Calculates the dot products for the forward vector & right vector
	float LocalForwardDot = CustomDotProductCalculation(DirectionUnitVector, GetActorForwardVector());
	float LocalRightDot = CustomDotProductCalculation(DirectionUnitVector, GetActorRightVector());

	// Converts the dot product into radians & then degrees
	float LocalYawResult = FMath::RadiansToDegrees(FMath::Atan2(LocalRightDot, LocalForwardDot));

	// Sets the new yaw rotation
	SetYaw(LocalYawResult);
}

void AGPRTurretBase::TurretPitchCalculation()
{
	// Gets the direction from the turrets muzzle to the target as a unit vector
	FVector DirectionUnitVector = ImpactPointPos - TurretMuzzleComponent->GetComponentLocation();

	// Normalizes the direction to a unit vector
	DirectionUnitVector.Normalize(0.0001f);

	// Calculates the dot product for the up vector
	float LocalUpDot = CustomDotProductCalculation(DirectionUnitVector, GetActorUpVector());

	// Converts the dot product into radians & then degrees
	float LocalUpDotDegrees = FMath::RadiansToDegrees(FMath::Acos(LocalUpDot));

	/* Normalizes the pitch range from 0 - 180 to 0 - 1 & then uses the normalized value
	 * to lerp between 90 (directly up) & -90 (directly down) */
	float LocalPitchNormalized = UKismetMathLibrary::NormalizeToRange(LocalUpDotDegrees, 0.f, 180.f);
	float LocalUpLerped = FMath::Lerp(90.f, -90.f, LocalPitchNormalized);

	// Sets the new pitch rotation
	SetPitch(LocalUpLerped);
}

bool AGPRTurretBase::IsTurretAimingAtTarget()
{
	// Gets the direction from the turret muzzle to the impact position & converts it to a unit vector
	FVector DirectionUnitVector = ImpactPointPos - TurretMuzzleComponent->GetComponentLocation();
	DirectionUnitVector.Normalize(0.0001f);

	// Gets the forward vector dot product from the turret muzzle to the target projectile
	float LocalForwardDotProduct = CustomDotProductCalculation(DirectionUnitVector, TurretMuzzleComponent->GetForwardVector());
	float LocalForwardDotProductInDegrees = FMath::RadiansToDegrees(FMath::Acos(LocalForwardDotProduct));

	// The angle at which the turret will consider the target to be in front of the turrets muzzle
	float ToleranceAngle = 0.5f;
	
	// Returns true if the turret is looking at the target within the tolerance angle
	if (LocalForwardDotProductInDegrees < ToleranceAngle)
	{
		return true;
	}

	// Returns false if the target is outside the aiming angle
	return false;
}

float AGPRTurretBase::CustomDotProductCalculation(const FVector& TargetDirection, const FVector& ActorAxisVector)
{
	// Multiplies the 2 vectors axis together for all 3 axes
	const float ResultX = TargetDirection.X * ActorAxisVector.X;
	const float ResultY = TargetDirection.Y * ActorAxisVector.Y;
	const float ResultZ = TargetDirection.Z * ActorAxisVector.Z;

	// Adds the results of all 3 together & returns the value
	return ResultX + ResultY + ResultZ;
}

float AGPRTurretBase::CustomVectorMagnitudeSquared(const FVector& LocalVector)
{
	// Squares all 3 vector axes values
	const float ResultX = FMath::Square(LocalVector.X);
	const float ResultY = FMath::Square(LocalVector.Y);
	const float ResultZ = FMath::Square(LocalVector.Z);

	// Returns the squared result
	return ResultX + ResultY + ResultZ;
}

void AGPRTurretBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	// If the overlapped object is an enemy than the target actor will now equal the overlapped enemy
	if (TObjectPtr<AGPREnemyBase> LocalCurrentEnemyTarget = Cast<AGPREnemyBase>(OtherActor))
	{
		// Adds the enemy to the enemy targets array
		EnemyTargetsArray.Add(LocalCurrentEnemyTarget);
	}
}

void AGPRTurretBase::OnEnemyDestroyed(AActor* DestroyedActor)
{
	// Checks if the destroyed actor is the turrets target enemy
	if (DestroyedActor == TargetEnemy)
	{
		// Removes the first element in the enemy target array allowing the turret to target the next enemy in line
		EnemyTargetsArray.RemoveAt(0);
		
		// Sets the enemy target to be nullptr
		TargetEnemy = nullptr;

		// Resets the turrets shooting timer & sets it to currently not shoot
		ResetTurretShooting();
	}
}

void AGPRTurretBase::UpdateTurretStatsBasedOnLevel()
{
	// Increases the size of the turret
	TurretBaseMesh->SetRelativeScale3D(FVector::One() + (TurretLevel - 1) * TurretScaleUpgradeIncrease);

	CurrentTurretFireRate = BaseTurretFireRate - (TurretLevel - 1) * TurretFireRateUpgradeIncrease; // Increases the fire rate of the turret
	CurrentTurretDamage = BaseTurretDamage + (TurretLevel - 1) * TurretDamageUpgradeIncrease; // Increases the damage of the turret

	// Updates the turret level widget text
	TurretLevelWidgetRef->UpdateTurretLevelText(TurretLevel);
}

void AGPRTurretBase::LevelUpTurret()
{
	// Will not execute if the turret is already at level 5
	if (TurretLevel < 5)
	{
		// Adds 1 level to the turrets overall level
		TurretLevel++;

		// Updates the turrets stats based on it's new level
		UpdateTurretStatsBasedOnLevel();

		// Resets the turrets shooting timer & sets it to currently not shoot
		ResetTurretShooting();
	}
}