// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Turret/GPRTurretLevel.h"

#include "Components/TextBlock.h"

void UGPRTurretLevel::UpdateTurretLevelText(const int32 NewLevel)
{
	TurretLevelText->SetText(FText::FromString(FString::Printf(TEXT("Turret Level: %d"), NewLevel)));
}