// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Turret/GPRTurretUpgradeShop.h"
#include "Actors/GPRTurretBase.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Core/GPRPlayerController.h"
#include "Core/GPRPlayerPawnBase.h"

void UGPRTurretUpgradeShop::NativeConstruct()
{
	Super::NativeConstruct();

	SetupFunctionBindings();
}

void UGPRTurretUpgradeShop::SetupFunctionBindings()
{
	UpgradeButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedUpgradeButton);
	ExitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedExitButton);
}

const bool UGPRTurretUpgradeShop::CanUpgradeTurret()
{		
	if (IsValid(PlayerControllerRef->PlayerPawnBaseRef))
	{		
		if (PlayerControllerRef->PlayerPawnBaseRef->PlayerCreditPoints >= UpgradeCreditsCost)
		{
			return true;
		}
	}

	return false;
}

void UGPRTurretUpgradeShop::UpgradeTurret()
{
	// Checks if the player can upgrade the turret & if the turret to upgrade ref is valid
	if (CanUpgradeTurret() && IsValid(TurretToUpgrade))
	{
		TurretToUpgrade->LevelUpTurret();
		PlayerControllerRef->PlayerPawnBaseRef->UpdateCreditPoints(-UpgradeCreditsCost);
	}
}

void UGPRTurretUpgradeShop::UpdateUpgradeShopStats(const float CurrentTurretDamage, const float UpgradedTurretDamage,
                                                   const float CurrentTurretFireRate, const float UpgradedTurretFireRate,
                                                   const int32 UpgradeCost)
{
	// Sets all text components
	UpgradeButtonText->SetText(FText::FromString(FString::Printf(TEXT("Upgrade: %d Credits"), UpgradeCost)));
	
	CurrentTurretDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), CurrentTurretDamage)));
	CurrentTurretFireRateText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), CurrentTurretFireRate)));
	
	TurretDamageUpgradeIncreaseText->SetText(FText::FromString(FString::Printf(TEXT("+%.2f"), UpgradedTurretDamage - CurrentTurretDamage)));
	TurretFireRateUpgradeIncreaseText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), UpgradedTurretFireRate - CurrentTurretFireRate)));

	// Sets all progress bars
	CurrentTurretDamageBar->SetPercent(CurrentTurretDamage / 50.f);
	CurrentTurretFireRateBar->SetPercent(UpgradedTurretFireRate / 50.f);
	
	UpgradedTurretDamageBar->SetPercent(UpgradedTurretDamage / 50.f);
	UpgradedTurretFireRateBar->SetPercent(CurrentTurretFireRate / 50.f);

	UpgradeCreditsCost = UpgradeCost;
	
	if (CanUpgradeTurret())
	{
		UpgradeButtonText->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		UpgradeButtonText->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void UGPRTurretUpgradeShop::DisplayUpgradeShopStatsAtMaxLevel(const float CurrentTurretDamage,
	const float CurrentTurretFireRate)
{
	// Set button disabled
	UpgradeButton->SetIsEnabled(false);
	
	// Sets all text components
	UpgradeButtonText->SetText(FText::FromString(FString::Printf(TEXT("Max Level"))));
	CurrentTurretDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), CurrentTurretDamage)));
	CurrentTurretFireRateText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), CurrentTurretFireRate)));
	TurretDamageUpgradeIncreaseText->SetVisibility(ESlateVisibility::Collapsed);
	TurretFireRateUpgradeIncreaseText->SetVisibility(ESlateVisibility::Collapsed);

	// Sets all progress bars
	CurrentTurretDamageBar->SetPercent(CurrentTurretDamage / 50.f);
	CurrentTurretFireRateBar->SetPercent(CurrentTurretFireRate / 50.f);
	
	UpgradedTurretDamageBar->SetPercent(0.f);
	UpgradedTurretFireRateBar->SetPercent(0.f);
}

void UGPRTurretUpgradeShop::OnClickedUpgradeButton()
{
	// If the player has enough credits then the turret will upgrade
	UpgradeTurret();

	// This will close the widget
	if (IsValid(OwningWidgetComponent))
	{
		OwningWidgetComponent->SetVisibility(false);
	}
}

void UGPRTurretUpgradeShop::OnClickedExitButton()
{
	// This will close the widget
	if (IsValid(OwningWidgetComponent))
	{
		OwningWidgetComponent->SetVisibility(false);
	}
}
