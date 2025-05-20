// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPRTurretBase.generated.h"

class UGPRTurretLevel;
class UWidgetComponent;
class AGPRPlayerController;
class AGPREnemyBase;
class USphereComponent;
class AGPRTurretProjectileBase;
class UNiagaraSystem;

UCLASS()
class GPR102_ASSESSMENT3_API AGPRTurretBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGPRTurretBase();

#pragma region // Components
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRootComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> TurretBaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> TurretHeadRotationPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> TurretBodyRotationPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> TurretHeadMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> TurretBodyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> TurretMuzzleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USphereComponent> TurretSphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> WidgetTurretLevelComponent;
#pragma endregion
	
#pragma region // Variables
	UPROPERTY(EditDefaultsOnly, Category = "Turret Stats")
	TSubclassOf<AGPRTurretProjectileBase> TurretProjectileClass;	
	
	UPROPERTY(EditDefaultsOnly, Category = "Turret Stats")
	float TurnSpeed = 0.0f;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Stats")
	int32 BaseUpgradeCost = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Stats")
	float BaseTurretFireRate = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Stats")
	float BaseTurretDamage = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Stats")
	float CurrentTurretFireRate = BaseTurretFireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Stats")
	float CurrentTurretDamage = BaseTurretDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret Stats")
	int32 TurretLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Stats")
	float TurretScaleUpgradeIncrease = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Stats")
	float TurretFireRateUpgradeIncrease = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Stats")
	float TurretDamageUpgradeIncrease = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Turret Effects")
	TObjectPtr<UNiagaraSystem> TurretShootingVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Turret Effects")
	TObjectPtr<USoundCue> TurretShootingSFX;

	UPROPERTY(EditDefaultsOnly, Category = "Turret Effects")
	TObjectPtr<USoundBase> TurretSpawnSFX;

protected:
	UPROPERTY(BlueprintReadOnly)
	float TimeToImpact = 0.f;

	UPROPERTY(BlueprintReadOnly)
	FVector ImpactPointPos = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<AGPREnemyBase>> EnemyTargetsArray;

	UPROPERTY()
	bool bIsShooting = false;

	UPROPERTY()
	FTimerHandle TurretFireTimerHandle;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AGPREnemyBase> TargetEnemy;

	UPROPERTY()
	float ProjectileSpeed = 0.f;

public:
	UPROPERTY()
	TObjectPtr<AGPRPlayerController> PlayerControllerRef;

	UPROPERTY()
	TObjectPtr<UGPRTurretLevel> TurretLevelWidgetRef;
#pragma endregion
	
#pragma region // Function Overrides
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region // Custom Functions
private:
	UFUNCTION()
	void PlayTurretShootingSFX();

	UFUNCTION()
	void PlayTurretShootingVFX();
	
	UFUNCTION()
	void ResetTurretShooting();
	
	UFUNCTION()
	void FindEnemyTarget();
	
	UFUNCTION()
	void SetupDefaultTurretProjectileSpeed();

	UFUNCTION()
	void ActivateTurretSystems();
	
	UFUNCTION()
	float TimeToImpactCalculation(const AActor* ActorToTarget, const USceneComponent* TurretMuzzle, const float TurretProjectileSpeed);

	UFUNCTION()
	FVector ImpactPosCalculation(const AActor* ActorToTarget) const;

	UFUNCTION()
	void TurretYawCalculation();

	UFUNCTION()
	void TurretPitchCalculation();

	UFUNCTION()
	bool IsTurretAimingAtTarget();

	UFUNCTION()
	static float CustomDotProductCalculation(const FVector& TargetDirection, const FVector& ActorAxisVector);

	// Returns the squared magnitude of a vector (avoids square rooting the result for better optimization)
	UFUNCTION()
	static float CustomVectorMagnitudeSquared(const FVector& LocalVector);

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void UpdateTurretStatsBasedOnLevel();

public:
	
	UFUNCTION()
	void LevelUpTurret();
	
	UFUNCTION()
	void Fire();

	UFUNCTION()
	void SetYaw(float TargetYaw) const;
	
	UFUNCTION()
	void SetPitch(float TargetPitch) const;
#pragma endregion
};