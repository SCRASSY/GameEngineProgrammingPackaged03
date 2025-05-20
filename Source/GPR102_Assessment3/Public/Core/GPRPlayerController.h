// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GPRPlayerController.generated.h"

class AGPRTurretBase;
struct FInputActionValue;
class UGPRGameOverMenu;
class AGPRTurretPlatform;
class UGPRPlayerHUD;
class AGPRPlayerPawnBase;
class UInputMappingContext;
class UInputAction;

UCLASS()
class GPR102_ASSESSMENT3_API AGPRPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// Holds the previous pos of the mouse screen coordinates
	FVector2D PrevMousePos = FVector2d::ZeroVector;

	UPROPERTY(EditDefaultsOnly)
	float CameraMoveSpeed = 500;

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGPRPlayerPawnBase> PlayerPawnBaseRef;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UGPRPlayerHUD> PlayerHUDClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<UInputMappingContext> InputMappingNavigationContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> MouseClick;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	TObjectPtr<UGPRPlayerHUD> PlayerHUDRef;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UGPRGameOverMenu> GameOverMenuClass;

private:
#pragma region // Custom Functions
	UFUNCTION()
	void GetPrevMousePos();

	UFUNCTION()
	void PanPlayerCamera();

	UFUNCTION()
	void SpawnPlayerHUD();

public:
	UFUNCTION()
	void OpenTurretShopUI(AGPRTurretPlatform* ClickedPlatform);

	UFUNCTION()
	void DisplayGameOverMenu();
#pragma endregion
};