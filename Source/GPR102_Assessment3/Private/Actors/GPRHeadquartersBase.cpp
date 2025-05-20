// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GPRHeadquartersBase.h"

#include "Actors/GPREnemyBase.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/GPRPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/GPRGameOverMenu.h"
#include "Widgets/GPRHeadquartersHealthWidget.h"


// Sets default values
AGPRHeadquartersBase::AGPRHeadquartersBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMeshComponent"));
	BuildingMeshComponent->SetupAttachment(RootComponent);

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->SetupAttachment(BuildingMeshComponent);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(BuildingMeshComponent);
}

// Called when the game starts or when spawned
void AGPRHeadquartersBase::BeginPlay()
{
	Super::BeginPlay();

	SetupFunctionBindings();
	SetupReferences();	
}

// Called every frame
void AGPRHeadquartersBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGPRHeadquartersBase::SetupReferences()
{
	// Casts the widget component to the headquarters health bar
	if (TObjectPtr<UGPRHeadquartersHealthWidget> LocalWidgetHealthBar = Cast<UGPRHeadquartersHealthWidget>(HealthBarWidgetComponent->GetWidget()))
	{
		WidgetHealthBarRef = LocalWidgetHealthBar;
	}
}

void AGPRHeadquartersBase::SetupFunctionBindings()
{
	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlapBoxCollisionComponent);
}

void AGPRHeadquartersBase::UpdateBuildingHealth()
{
	// Takes 10 health away from the building
	CurrentBuildingHealth -= 10.f;

	// If the buildings health after taking damage is equal to or less than 0 than the game will pause / end
	if (CurrentBuildingHealth / MaxBuildingHealth <= 0.f)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		// Casts to the player controller in order to create the game over widget
		if (TObjectPtr<AGPRPlayerController> LocalPlayerControllerRef = Cast<AGPRPlayerController>(
			UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			// Displays the game over menu when the player loses
			LocalPlayerControllerRef->DisplayGameOverMenu();
		}
	}
	
	if (IsValid(WidgetHealthBarRef))
	{
		// Updates the widgets health progress bar based on the building health
		WidgetHealthBarRef->UpdateHealthBar(CurrentBuildingHealth / MaxBuildingHealth);
	}
}

void AGPRHeadquartersBase::OnComponentBeginOverlapBoxCollisionComponent(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// First checks if the overlapped actor inherits from enemy base actor
	if (TObjectPtr<AGPREnemyBase> LocalOverlappedEnemy = Cast<AGPREnemyBase>(OtherActor))
	{
		// Destroys the enemy actor that has hit this building
		LocalOverlappedEnemy->Destroy();

		// Updates the buildings health
		UpdateBuildingHealth();
	}
}
