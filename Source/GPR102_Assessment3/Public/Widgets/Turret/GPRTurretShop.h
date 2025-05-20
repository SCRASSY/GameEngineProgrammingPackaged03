// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRTurretShop.generated.h"

class UScrollBox;
class UGPRTurretInfoStats;
class UButton;
class UGPRTurretInfoContainer;
class UOverlay;
class AGPRTurretPlatform;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRTurretShop : public UUserWidget
{
	GENERATED_BODY()

protected:
#pragma region // Function Overrides
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region // Custom Functions
	UFUNCTION()
	void AddTurretTypesToShop();

	UFUNCTION()
	void SetupFunctionBindings();
#pragma endregion

#pragma region // Function Bindings
	UFUNCTION()
	void OnClickedExitButton();
#pragma endregion
	
public:
#pragma region // Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> TurretInfoOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UGPRTurretInfoStats> WBP_TurretInfoStats;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UScrollBox> TurretSelectionScrollBox;
#pragma endregion

#pragma region // Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 PlayerCreditPointsValue = 0;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AGPRTurretPlatform> ChosenPlatform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDataTable> TurretTypeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGPRTurretInfoContainer> TurretInfoContainerClassToAdd;
#pragma endregion
};