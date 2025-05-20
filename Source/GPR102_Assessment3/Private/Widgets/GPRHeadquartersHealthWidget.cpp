// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GPRHeadquartersHealthWidget.h"
#include "Components/ProgressBar.h"

void UGPRHeadquartersHealthWidget::UpdateHealthBar(const float NewHealth)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health: %.1f"), NewHealth));
	
	// Updates the health bar of the building
	HealthProgressBar->SetPercent(NewHealth);
}
