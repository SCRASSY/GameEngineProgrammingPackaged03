// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRCreditPointsWidget.generated.h"

class UTextBlock;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRCreditPointsWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CreditsText;

	UFUNCTION(BlueprintCallable)
	void UpdateCreditPointsText(const int NewValue);
};
