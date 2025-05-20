// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRGameOverMenu.generated.h"


class UButton;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRGameOverMenu : public UUserWidget
{
	GENERATED_BODY()

#pragma region // Function Overrides
	virtual void NativeConstruct() override;
#pragma endregion
	
public:
#pragma region // Components
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget)) 
	TObjectPtr<UButton> MainMenuButton;
#pragma endregion

protected:
#pragma region // Custom Functions
	UFUNCTION()
	void SetupFunctionBindings();
#pragma endregion

#pragma region // Function Bindings
	UFUNCTION()
	void OnClickedRestartButton();

	UFUNCTION()
	void OnClickedMainMenuButton();
#pragma endregion
};
