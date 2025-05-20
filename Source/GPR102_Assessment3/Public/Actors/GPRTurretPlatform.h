// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPRTurretPlatform.generated.h"

class UWidgetComponent;
class AGPRTurretBase;
class AGPRPlayerController;

UCLASS()
class GPR102_ASSESSMENT3_API AGPRTurretPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGPRTurretPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region // Components	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> PlatformMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> WidgetCursorSelectionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> WidgetUpgradeShopComponent;
#pragma endregion
	
#pragma region // Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<AGPRPlayerController> PlayerControllerRef;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<AGPRTurretBase> TurretRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float UpgradeIncreaseAmount = 5.f;
#pragma endregion

#pragma region // Custom Functions
private:
	UFUNCTION()
	void SetupPlayerControllerReference();

	UFUNCTION()
	void SetupFunctionBindings();

	UFUNCTION(BlueprintCallable)
	void UpdateWidgetDrawSize(const float Alpha);

	UFUNCTION()
	void OpenTurretUpgradeShopUI();

public:
	UFUNCTION()
	void SpawnTurret(const TSubclassOf<AGPRTurretBase>& TurretToSpawn, const int32 CreditPointsCost);
#pragma endregion
	
#pragma region // Function Bindings
private:
	UFUNCTION()
	void OnBeginCursorOverPlatformMesh(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnEndCursorOverPlatformMesh(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnClickedPlatformMesh(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
#pragma endregion

#pragma region // Implementable Events
public:
	UFUNCTION(BlueprintImplementableEvent)
	void TriggerWidgetAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void StopWidgetAnim();
#pragma endregion
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
