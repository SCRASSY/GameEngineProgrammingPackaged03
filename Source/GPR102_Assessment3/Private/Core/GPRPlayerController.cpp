// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GPRPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Core/GPRPlayerPawnBase.h"
#include "Widgets/GPRPlayerHUD.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Widgets/GPRGameOverMenu.h"
#include "Widgets/Turret/GPRTurretShop.h"
#include "Widgets/Turret/GPRTurretInfoStats.h"

void AGPRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bShowMouseCursor = true; // Enables the mouse cursor so it can be seen by the player
	PlayerPawnBaseRef = Cast<AGPRPlayerPawnBase>(GetPawn()); // Gets a ref to the current player pawn being controlled by this controller

	// Setup for controller enhanced input
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingNavigationContext, 0);
	}

	// Setup player controller input bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MouseClick, ETriggerEvent::Started, this, &ThisClass::GetPrevMousePos);
		EnhancedInputComponent->BindAction(MouseClick, ETriggerEvent::Triggered, this, &ThisClass::PanPlayerCamera);
	}

	// Spawns in the players HUD
	SpawnPlayerHUD();
}

void AGPRPlayerController::GetPrevMousePos()
{
	PrevMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
}

void AGPRPlayerController::PanPlayerCamera()
{
	// Declares a new mouse position which will be slightly different to the old saved mouse position
	FVector2D LocalNewMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	// Gets the direction from the mouses old position to the new position
	FVector2D LocalVectorDirection = LocalNewMousePos - PrevMousePos;
	FVector LocalFinalPlayerPos = FVector(LocalVectorDirection.Y, -LocalVectorDirection.X, 0.f) * CameraMoveSpeed;

	// Calls the players move function & uses the passed value as it's new location
	PlayerPawnBaseRef->MovePlayerPawn(LocalFinalPlayerPos);

	// Updates the previous position of the mouse
	PrevMousePos = LocalNewMousePos;
}

void AGPRPlayerController::SpawnPlayerHUD()
{
	if (PlayerHUDClass != nullptr)
	{
		// Creates the players HUD widget
		PlayerHUDRef = CreateWidget<UGPRPlayerHUD>(this, PlayerHUDClass);
		if (PlayerHUDRef)
		{
			// Adds the player HUD widget to the player assigned to this player controller
			PlayerHUDRef->AddToViewport();
		}
	}
}

void AGPRPlayerController::OpenTurretShopUI(AGPRTurretPlatform* ClickedPlatform)
{
	PlayerHUDRef->WBP_TurretShop->SetVisibility(ESlateVisibility::Visible);
	PlayerHUDRef->WBP_TurretShop->PlayerCreditPointsValue = PlayerPawnBaseRef->PlayerCreditPoints;
	PlayerHUDRef->WBP_TurretShop->WBP_TurretInfoStats->SelectedPlatform = ClickedPlatform;
}

void AGPRPlayerController::DisplayGameOverMenu()
{
	if (IsValid(GameOverMenuClass))
	{
		if (TObjectPtr<UGPRGameOverMenu> LocalGameOverWidget = CreateWidget<UGPRGameOverMenu>(
	this, GameOverMenuClass))
		{
			LocalGameOverWidget->AddToViewport();
		}
	}
}
