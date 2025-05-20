// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GPRCreditPointsWidget.h"
#include "Components/TextBlock.h"

void UGPRCreditPointsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGPRCreditPointsWidget::UpdateCreditPointsText(const int NewValue)
{
	// Checks if "CreditsText" is linked up correctly
	if (ensure(CreditsText != nullptr))
	{
		// Creates a new formatted text from the new input int value & updates the credit score
		const FText FormattedText = FText::FromString(FString::Printf(TEXT("Credits: %d"), NewValue));
		CreditsText->SetText(FormattedText);
	}
}