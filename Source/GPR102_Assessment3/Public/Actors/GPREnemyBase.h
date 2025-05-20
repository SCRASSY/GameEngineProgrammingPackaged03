// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPREnemyBase.generated.h"

class UNiagaraSystem;
class UGPREnemyHealthBarWidget;
class AGPRPlayerController;
class UWidgetComponent;
class USplineComponent;

UCLASS()
class GPR102_ASSESSMENT3_API AGPREnemyBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGPREnemyBase();

protected:
#pragma region // Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> EnemyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBarWidget;
#pragma endregion

#pragma region // Variables
	// The time it takes for the enemy to move from the start to the end
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats", meta = (ExposeOnSpawn = true))
	float EnemyMovementSpeed = 5.f;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Stats")
	float CurrentHealth = MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USplineComponent> SplineRef;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AGPRPlayerController> PlayerControllerRef;

	UPROPERTY()
	TObjectPtr<UGPREnemyHealthBarWidget> EnemyHealthBarWidgetRef;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Effects")
	TObjectPtr<UNiagaraSystem> EnemyDeathVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Effects")
	TObjectPtr<USoundCue> EnemyDeathSFX;
#pragma endregion

#pragma region // Function Overrides
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion
	
#pragma region // Custom Functions
protected:
	UFUNCTION()
	void SetupEnemyHealthBarWidgetRef();
	
	UFUNCTION()
	void UpdateEnemyHealthBar();
	
	UFUNCTION()
	void SetupPlayerControllerReference();
	
	UFUNCTION()
	void SetupFunctionBindings();
	
	UFUNCTION(BlueprintCallable)
	void MoveEnemyAlongSpline(const float Progress);
#pragma endregion

#pragma region // Function Bindings
	UFUNCTION()
	void OnDestroyedEnemyBase(AActor* DestroyedActor);

	UFUNCTION()
	void AnyDamageEnemyBase(AActor* DamagedActor, float Damage,const class UDamageType* DamageType,
	                        class AController* InstigatedBy, AActor* DamageCauser);
#pragma endregion
};