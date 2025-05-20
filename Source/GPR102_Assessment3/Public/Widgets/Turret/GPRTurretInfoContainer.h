// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GPRTurretInfoContainer.generated.h"

class UImage;
class UTextBlock;
class UGPRTurretShop;
class UButton;

UCLASS()
class GPR102_ASSESSMENT3_API UGPRTurretInfoContainer : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ContainerButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TurretNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TurretImageIcon;

#pragma region // Variables
	UPROPERTY(BlueprintReadWrite)
	FName TurretRowName;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UGPRTurretShop> ParentWidgetRef;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDataTable> DataTableRef;
#pragma endregion
	
protected:
#pragma region // Function Bindings
	UFUNCTION()
	void OnHoveredContainerButton();

	UFUNCTION()
	void OnUnhoveredContainerButton();

	UFUNCTION()
	void OnClickedContainerButton();
#pragma endregion
};
