// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRTurretInfoStats.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;
class UButton;
class AGPRPlayerController;
class AGPRTurretBase;
class AGPRTurretPlatform;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRTurretInfoStats : public UUserWidget
{
	GENERATED_BODY()

protected:
#pragma region // Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BuyButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> BuyButtonText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> DamageStatBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> FireRateStatBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageValueText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> FireRateValueText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelValueText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TurretIconImage;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TurretNameText;
#pragma endregion

#pragma region // Function Overrides
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region // Custom Functions
	UFUNCTION()
	void SetupPlayerControllerRef();

	UFUNCTION()
	void SetupFunctionBindings();

	UFUNCTION()
	void BuyTurret();

	UFUNCTION()
	bool CanBuyTurret();

public:
	UFUNCTION()
	void UpdateWidgetInfo(const FName& Name,
	                      const int32 Level,
	                      const float Damage,
	                      const float FireRate,
	                      UTexture2D* Icon,
	                      const TSubclassOf<AGPRTurretBase>& TurretClass,
	                      const int32 Credits);
#pragma endregion
	
#pragma region // Function Bindings
	UFUNCTION()
	void OnClickedBuyButton();
#pragma endregion
	
public:
#pragma region // Variables
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AGPRTurretPlatform> SelectedPlatform;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AGPRTurretBase> TurretClassToSpawn;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AGPRPlayerController> PlayerControllerRef;

	UPROPERTY(BlueprintReadWrite)
	int32 CreditPointsCost = 0;
#pragma endregion
};
