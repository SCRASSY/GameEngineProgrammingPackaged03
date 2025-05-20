// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GPRPlayerPawnBase.generated.h"

UCLASS()
class GPR102_ASSESSMENT3_API AGPRPlayerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGPRPlayerPawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FVector PlayerStartPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PlayerMovementRangeXY = 1000.f;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 PlayerCreditPoints = 0;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void MovePlayerPawn(const FVector& NewPlayerPawnPos);
	
	UFUNCTION(BlueprintCallable)
	void UpdateCreditPoints(const int CreditPointsToAdd);
};