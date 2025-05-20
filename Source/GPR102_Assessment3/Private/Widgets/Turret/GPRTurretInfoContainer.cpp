// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Turret/GPRTurretInfoContainer.h"
#include "Actors/GPRTurretBase.h"
#include "Widgets/Turret/GPRTurretInfoStats.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Structs/TurretData.h"
#include "Widgets/Turret/GPRTurretShop.h"

void UGPRTurretInfoContainer::NativeConstruct()
{
	Super::NativeConstruct();

	if (ContainerButton)
	{
		ContainerButton->OnHovered.AddDynamic(this, &ThisClass::UGPRTurretInfoContainer::OnHoveredContainerButton);
		ContainerButton->OnUnhovered.AddDynamic(this, &ThisClass::UGPRTurretInfoContainer::OnUnhoveredContainerButton);
		ContainerButton->OnClicked.AddDynamic(this, &ThisClass::UGPRTurretInfoContainer::OnClickedContainerButton);
	}
}

void UGPRTurretInfoContainer::OnHoveredContainerButton()
{
	ContainerButton->SetBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.2f));
}

void UGPRTurretInfoContainer::OnUnhoveredContainerButton()
{
	ContainerButton->SetBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}

void UGPRTurretInfoContainer::OnClickedContainerButton()
{
	ParentWidgetRef->TurretInfoOverlay->SetVisibility(ESlateVisibility::Visible);

	if (const auto LocalTurretData = DataTableRef->FindRow<FTurretData>(TurretRowName, ""))
	{
		// Checks if the parent widgets turret info stats child widget is valid
		if (IsValid(ParentWidgetRef->WBP_TurretInfoStats))
		{
			// Organizes the LocalTurretData variables into easier to manage local variables
			const FName LocalName = LocalTurretData->Name;
			const int32 LocalLevel = LocalTurretData->Level;
			const float LocalDamage = LocalTurretData->TurretClass->GetDefaultObject<AGPRTurretBase>()->CurrentTurretDamage;
			const float LocalFireRate = LocalTurretData->TurretClass->GetDefaultObject<AGPRTurretBase>()->CurrentTurretFireRate;
			const TObjectPtr<UTexture2D> LocalIcon = LocalTurretData->Icon;
			const TSubclassOf<AGPRTurretBase> LocalTurretBaseClass = LocalTurretData->TurretClass;
			const int32 LocalCredits = LocalTurretData->CreditCost;

			// Updates the turret info stats widget based on which turret icon the player clicked on in the shop
			ParentWidgetRef->WBP_TurretInfoStats->UpdateWidgetInfo(LocalName, LocalLevel, LocalDamage, LocalFireRate,
			                                                       LocalIcon, LocalTurretBaseClass, LocalCredits);
		}
	}
}