// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRTurretLevel.generated.h"

class UTextBlock;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRTurretLevel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TurretLevelText;

#pragma region // Custom Functions
	UFUNCTION()
	void UpdateTurretLevelText(const int32 NewLevel);
#pragma endregion
};
