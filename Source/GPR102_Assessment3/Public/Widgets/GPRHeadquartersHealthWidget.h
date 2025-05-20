// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRHeadquartersHealthWidget.generated.h"

class UProgressBar;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRHeadquartersHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

	UFUNCTION()
	void UpdateHealthBar(const float NewHealth);

#pragma region // Variables
	
#pragma endregion
};
