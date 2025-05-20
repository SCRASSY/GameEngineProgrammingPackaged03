// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRTurretUpgradeShop.generated.h"

class UWidgetComponent;
class AGPRTurretBase;
class AGPRPlayerController;
class UProgressBar;
class UTextBlock;
class UButton;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRTurretUpgradeShop : public UUserWidget
{
	GENERATED_BODY()

public:
#pragma region // Components
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> UpgradeButton;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeButtonText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	// Turret damage components
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> UpgradedTurretDamageBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> CurrentTurretDamageBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTurretDamageText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TurretDamageUpgradeIncreaseText;

	// Turret fire rate components
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> UpgradedTurretFireRateBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> CurrentTurretFireRateBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTurretFireRateText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TurretFireRateUpgradeIncreaseText;
#pragma endregion

#pragma region // Variables
	UPROPERTY()
	TObjectPtr<AGPRPlayerController> PlayerControllerRef;

	UPROPERTY()
	TObjectPtr<AGPRTurretBase> TurretToUpgrade;
	
	UPROPERTY()
	TObjectPtr<UWidgetComponent> OwningWidgetComponent;

	UPROPERTY()
	int32 UpgradeCreditsCost = 0;
#pragma endregion

#pragma region // Function Overrides
	virtual void NativeConstruct() override;
#pragma endregion
	
#pragma region // Custom Functions
	UFUNCTION()
	void SetupFunctionBindings();

	UFUNCTION()
	const bool CanUpgradeTurret();

	UFUNCTION()
	void UpgradeTurret();
	
	UFUNCTION()
	void UpdateUpgradeShopStats(const float CurrentTurretDamage, const float UpgradedTurretDamage,
	                            const float CurrentTurretFireRate, const float UpgradedTurretFireRate,
	                            const int32 UpgradeCost);

	UFUNCTION()
	void DisplayUpgradeShopStatsAtMaxLevel(const float CurrentTurretDamage, const float CurrentTurretFireRate);
#pragma endregion

#pragma region // Function Bindings
	UFUNCTION()
	void OnClickedUpgradeButton();

	UFUNCTION()
	void OnClickedExitButton();
#pragma endregion
};
