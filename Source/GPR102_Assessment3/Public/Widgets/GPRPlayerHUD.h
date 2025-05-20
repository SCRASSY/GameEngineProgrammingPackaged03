// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRPlayerHUD.generated.h"

class UGPRTurretShop;
class UGPRCreditPointsWidget;
class UGPRHeadquartersHealthWidget;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGPRCreditPointsWidget> WBP_CreditPoints;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UGPRTurretShop> WBP_TurretShop;
};
