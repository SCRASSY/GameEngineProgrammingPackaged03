// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Turret/GPRTurretShop.h"
#include "Widgets/Turret/GPRTurretInfoContainer.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "Structs/TurretData.h"

void UGPRTurretShop::NativeConstruct()
{
	Super::NativeConstruct();
	
	AddTurretTypesToShop(); // Adds the available turrets to purchase to the turret shop
	SetupFunctionBindings(); // Sets up all function bindings
}

void UGPRTurretShop::AddTurretTypesToShop()
{
	// Gets an array of names from the turret type data table
	const TArray<FName> LocalRowNames = TurretTypeDataTable->GetRowNames();

	// Loops through each row name from the array of names
	for (FName LocalRowName : LocalRowNames)
	{
		// Gets the turret data based on the current row name
		const auto LocalTurretDataResult = TurretTypeDataTable->FindRow<FTurretData>(LocalRowName, "");

		// Creates an info container widget to be added to the turret shop list
		TObjectPtr<UGPRTurretInfoContainer> LocalWidgetToAdd = CreateWidget<UGPRTurretInfoContainer>(this, TurretInfoContainerClassToAdd);

		// Adjusts the created widgets params
		LocalWidgetToAdd->ParentWidgetRef = this;
		LocalWidgetToAdd->TurretRowName = LocalRowName;
		LocalWidgetToAdd->TurretImageIcon->SetBrushFromTexture(LocalTurretDataResult->Icon);
		LocalWidgetToAdd->TurretNameText->SetText(FText::FromName(LocalTurretDataResult->Name));
		LocalWidgetToAdd->DataTableRef = TurretTypeDataTable;

		// Adds the created widget to the turret shop & stores the returned value
		auto LocalChildWidget = TurretSelectionScrollBox->AddChild(LocalWidgetToAdd);

		/* Casts the returned added widget to a scroll box slot in order to
		 * adjust the added widgets padding & horizontal alignment */
		if (auto LocalScrollBoxSlot = Cast<UScrollBoxSlot>(LocalChildWidget))
		{
			LocalScrollBoxSlot->SetPadding(FMargin(2.0f, 2.0f, 2.0f, 2.0f));
			LocalScrollBoxSlot->SetHorizontalAlignment(HAlign_Center);
		}
	}
}

void UGPRTurretShop::SetupFunctionBindings()
{
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedExitButton);
}

void UGPRTurretShop::OnClickedExitButton()
{
	TurretInfoOverlay->SetVisibility(ESlateVisibility::Hidden);
	SetVisibility(ESlateVisibility::Hidden);
}
