// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPRTurretProjectileBase.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class GPR102_ASSESSMENT3_API AGPRTurretProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGPRTurretProjectileBase();

#pragma region // Components
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
#pragma endregion

#pragma region // Variables
	UPROPERTY()
	float ProjectileDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Effects")
	TObjectPtr<UNiagaraSystem> ProjectileHitVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Effects")
	TObjectPtr<USoundCue> ProjectileHitSFX;
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
	UFUNCTION()
	void SetupFunctionBindings();
#pragma endregion

#pragma region // Function Bindings
	UFUNCTION()
	void OnComponentHitProjectileMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
	                                  const FHitResult& Hit);
	
#pragma endregion
};
