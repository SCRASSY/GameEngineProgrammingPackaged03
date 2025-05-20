// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Turret/GPRTurretInfoStats.h"
#include "Core/GPRPlayerController.h"
#include "Core/GPRPlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/GPRTurretPlatform.h"
#include "Actors/GPRTurretBase.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Widgets/GPRPlayerHUD.h"
#include "Widgets/Turret/GPRTurretShop.h"

void UGPRTurretInfoStats::NativeConstruct()
{
	Super::NativeConstruct();

	SetupPlayerControllerRef();
	SetupFunctionBindings();
}

void UGPRTurretInfoStats::SetupPlayerControllerRef()
{
	// Does a cast to the player controller & saves the player controller being used in the player controller ref
	if (TObjectPtr<AGPRPlayerController> LocalPlayerController = Cast<AGPRPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerControllerRef = LocalPlayerController;
	}
}

void UGPRTurretInfoStats::SetupFunctionBindings()
{
	BuyButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedBuyButton);
}

void UGPRTurretInfoStats::BuyTurret()
{
	// Null ptr checks
	if (IsValid(SelectedPlatform))
	{
		SelectedPlatform->SpawnTurret(TurretClassToSpawn, CreditPointsCost);

		// Null ptr check
		if (IsValid(PlayerControllerRef->PlayerHUDRef->WBP_TurretShop))
		{
			PlayerControllerRef->PlayerHUDRef->WBP_TurretShop->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

bool UGPRTurretInfoStats::CanBuyTurret()
{
	const int32 LocalPlayerCreditPoints = PlayerControllerRef->PlayerPawnBaseRef->PlayerCreditPoints;

	// Returns true if the player has enough credits to buy the turret
	if (LocalPlayerCreditPoints >= CreditPointsCost)
	{
		return true;
	}

	// Returns false if the player does not have enough credits to buy the turret
	return false;
}

void UGPRTurretInfoStats::UpdateWidgetInfo(const FName& Name,
                                           const int32 Level,
                                           const float Damage,
                                           const float FireRate,
                                           UTexture2D* Icon,
                                           const TSubclassOf<AGPRTurretBase>& TurretClass,
                                           const int32 Credits)
{
	// Updates all the widget component values
	TurretNameText->SetText(FText::FromName(Name));
	DamageValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Damage)));
	FireRateValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), FireRate)));
	LevelValueText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), Level)));
	BuyButtonText->SetText(FText::FromString(FString::Printf(TEXT("Buy: %d Credits"), Credits)));
	DamageStatBar->SetPercent(Damage / 50.f);
	FireRateStatBar->SetPercent(FireRate / 50.f);
	TurretIconImage->SetBrushFromTexture(Icon);
	
	TurretClassToSpawn = TurretClass;
	CreditPointsCost = Credits;

	if (CanBuyTurret())
	{
		BuyButtonText->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		BuyButtonText->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void UGPRTurretInfoStats::OnClickedBuyButton()
{
	// Checks if the player has enough credits to buy the turret
	if (CanBuyTurret())
	{
		// Buys the turret
		BuyTurret();
	}
}
