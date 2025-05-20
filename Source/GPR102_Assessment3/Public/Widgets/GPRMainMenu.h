// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRMainMenu.generated.h"

class UButton;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
#pragma region // Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;
#pragma endregion

#pragma region // Function Overrides
	virtual void NativeConstruct() override;
#pragma endregion
	
#pragma region // Custom Functions
	UFUNCTION()
	void SetupFunctionBindings();
#pragma endregion

#pragma region // Function Bindings
	UFUNCTION()
	void OnClickedStartGameButton();

	UFUNCTION()
	void OnClickedExitGameButton();
#pragma endregion
};
