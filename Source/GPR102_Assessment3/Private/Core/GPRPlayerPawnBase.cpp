// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GPRPlayerPawnBase.h"
#include "Core/GPRPlayerController.h"
#include "Widgets/GPRCreditPointsWidget.h"
#include "Widgets/GPRPlayerHUD.h"

// Sets default values
AGPRPlayerPawnBase::AGPRPlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGPRPlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();

	// Stores the players starting location in world space
	PlayerStartPos = GetActorLocation();

	// Updates the players credit points score UI at the start of the game
	UpdateCreditPoints(0);
}

// Called every frame
void AGPRPlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGPRPlayerPawnBase::MovePlayerPawn(const FVector& NewPlayerPawnPos)
{
	// Creates a local vector variable which will equal the new location where the player is trying to move towards
	FVector LocalNewLocation = GetActorLocation() + NewPlayerPawnPos;
	
	// Stores the min & max players movement ranges for better readability & organization
	const float MinRangeX = PlayerStartPos.X - PlayerMovementRangeXY;
	const float MaxRangeX = PlayerStartPos.X + PlayerMovementRangeXY;
	const float MinRangeY = PlayerStartPos.Y - PlayerMovementRangeXY;
	const float MaxRangeY = PlayerStartPos.Y + PlayerMovementRangeXY;
	
	/* Clamps the x & y values of the player pawns location vector
	 * This will stop the player from moving outside the boundaries that are set*/
	LocalNewLocation.X = FMath::Clamp(LocalNewLocation.X, MinRangeX, MaxRangeX);
	LocalNewLocation.Y = FMath::Clamp(LocalNewLocation.Y, MinRangeY, MaxRangeY);

	// Sets the actor to the new clamped location
	SetActorLocation(LocalNewLocation);
}

void AGPRPlayerPawnBase::UpdateCreditPoints(const int CreditPointsToAdd)
{
	// Adds the given credit points to the players credit points score
	PlayerCreditPoints += CreditPointsToAdd;

	// Cast the pawns player controller to the custom player controller used in the runtime game mode
	if (TObjectPtr<AGPRPlayerController> PlayerController = Cast<AGPRPlayerController>(GetController()))
	{
		// Updates the player HUD credit points widget to sync with the players credit points score
		PlayerController->PlayerHUDRef->WBP_CreditPoints->UpdateCreditPointsText(PlayerCreditPoints);
	}
}