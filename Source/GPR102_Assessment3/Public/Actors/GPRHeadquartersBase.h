// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPRHeadquartersBase.generated.h"

class UGPRHeadquartersHealthWidget;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class GPR102_ASSESSMENT3_API AGPRHeadquartersBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGPRHeadquartersBase();

#pragma region // Function Overrides
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region // Components
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> BuildingMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxCollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;
#pragma endregion

#pragma region // Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxBuildingHealth = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentBuildingHealth = MaxBuildingHealth;

	UPROPERTY()
	TObjectPtr<UGPRHeadquartersHealthWidget> WidgetHealthBarRef;
#pragma endregion

#pragma region // Custom Functions
	UFUNCTION()
	void SetupReferences();
	
	UFUNCTION()
	void SetupFunctionBindings();

	UFUNCTION()
	void UpdateBuildingHealth();
#pragma endregion
	
#pragma region // Function Bindings
	UFUNCTION()
	void OnComponentBeginOverlapBoxCollisionComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                                  bool bFromSweep, const FHitResult& SweepResult);
#pragma endregion
};
