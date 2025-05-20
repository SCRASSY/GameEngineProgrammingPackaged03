// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GPRTurretPlatform.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GPRPlayerController.h"
#include "Actors/GPRTurretBase.h"
#include "Core/GPRPlayerPawnBase.h"
#include "Widgets/GPRPlayerHUD.h"
#include "Widgets/Turret/GPRTurretShop.h"
#include "Widgets/Turret/GPRTurretUpgradeShop.h"


// Sets default values
AGPRTurretPlatform::AGPRTurretPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMeshComponent"));
	PlatformMeshComponent->SetupAttachment(RootComponent);

	WidgetCursorSelectionComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetCursorSelectionComponent"));
	WidgetCursorSelectionComponent->SetupAttachment(PlatformMeshComponent);

	WidgetUpgradeShopComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetUpgradeShopComponent"));
	WidgetUpgradeShopComponent->SetupAttachment(PlatformMeshComponent);
}

// Called when the game starts or when spawned
void AGPRTurretPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	SetupPlayerControllerReference();
	SetupFunctionBindings();
}

void AGPRTurretPlatform::SetupPlayerControllerReference()
{
	// Casts the worlds player controller to the custom player controller & sets it as the player controller reference
	if (TObjectPtr<AGPRPlayerController> LocalPlayerController = Cast<AGPRPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerControllerRef = LocalPlayerController;
	}
}

void AGPRTurretPlatform::SetupFunctionBindings()
{
	PlatformMeshComponent->OnBeginCursorOver.AddDynamic(this, &ThisClass::AGPRTurretPlatform::OnBeginCursorOverPlatformMesh);
	PlatformMeshComponent->OnEndCursorOver.AddDynamic(this, &ThisClass::AGPRTurretPlatform::OnEndCursorOverPlatformMesh);
	PlatformMeshComponent->OnClicked.AddDynamic(this, &ThisClass::AGPRTurretPlatform::OnClickedPlatformMesh);
}

void AGPRTurretPlatform::UpdateWidgetDrawSize(const float Alpha)
{
	// Lerps using a timeline
	const float SizeX = FMath::Lerp(400.f, 500.f, Alpha);
	const float SizeY = FMath::Lerp(400.f, 500.f, Alpha);
	
	// Sets the size of the selection widget
	WidgetCursorSelectionComponent->SetDrawSize(FVector2d(SizeX, SizeY));
}

void AGPRTurretPlatform::OpenTurretUpgradeShopUI()
{
	// Shows the turret upgrade shop
	WidgetUpgradeShopComponent->SetVisibility(true);

	// Casts the widget components currently used widget to the turret upgrade shop widget
	if (TObjectPtr<UGPRTurretUpgradeShop> LocalUpgradeShop = Cast<UGPRTurretUpgradeShop>(WidgetUpgradeShopComponent->GetWidget()))
	{
		// Passes references through to the turret upgrade shop widget
		LocalUpgradeShop->TurretToUpgrade = TurretRef;
		LocalUpgradeShop->OwningWidgetComponent = WidgetUpgradeShopComponent;
		LocalUpgradeShop->PlayerControllerRef = PlayerControllerRef;

		// If the turret is already level 5 (max level) then the upgrade UI will be changed
		if (TurretRef->TurretLevel == 5)
		{
			LocalUpgradeShop->DisplayUpgradeShopStatsAtMaxLevel(TurretRef->CurrentTurretDamage,
			                                                    TurretRef->CurrentTurretFireRate);
			return;
		}

		// Declares local variables for organization
		const float CurrenTurretDamage = TurretRef->CurrentTurretDamage;
		const float CurrenTurretFireRate = TurretRef->CurrentTurretFireRate;
		const float UpgradedTurretDamage = CurrenTurretDamage + TurretRef->TurretDamageUpgradeIncrease;
		const float UpgradedTurretFireRate = CurrenTurretFireRate - TurretRef->TurretFireRateUpgradeIncrease;

		/* Modifies the upgrade cost of the turret based on it's current level
		 * The higher the turret level, the higher the cost to upgrade it */
		const int32 UpgradeCost = TurretRef->BaseUpgradeCost + (TurretRef->TurretLevel - 1) * UpgradeIncreaseAmount;
		
		LocalUpgradeShop->UpdateUpgradeShopStats(CurrenTurretDamage, UpgradedTurretDamage, CurrenTurretFireRate, UpgradedTurretFireRate, UpgradeCost);
	}
}

void AGPRTurretPlatform::SpawnTurret(const TSubclassOf<AGPRTurretBase>& TurretToSpawn, const int32 CreditPointsCost)
{
	/* Declares a local vector & adds 30 to the z axis in order for the
	* turret to be placed on top of the platform properly */
	FVector LocalLocation = GetActorLocation();
	LocalLocation.Z += 45.f;

	// Spawn parameters for the turret
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Null ptr checks
	if (IsValid(TurretToSpawn))
	{
		// Sets the platforms turret reference to be the turret the player just purchased & spawned in
		TurretRef = GetWorld()->SpawnActor<AGPRTurretBase>(TurretToSpawn, LocalLocation, FRotator::ZeroRotator, SpawnParams);	
	}

	// Null ptr checks
	if (IsValid(TurretRef))
	{
		// Sets the spawned turrets player controller ref
		TurretRef->PlayerControllerRef = PlayerControllerRef;

		// Null ptr checks
		if (IsValid(PlayerControllerRef->PlayerPawnBaseRef))
		{
			// Takes away the amount of credits it cost for the turret from the players credits
			PlayerControllerRef->PlayerPawnBaseRef->UpdateCreditPoints(-CreditPointsCost);
		}

		// Null ptr checks
		if (IsValid(PlayerControllerRef->PlayerHUDRef->WBP_TurretShop))
		{
			// Updates the credits UI
			PlayerControllerRef->PlayerHUDRef->WBP_TurretShop->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AGPRTurretPlatform::OnBeginCursorOverPlatformMesh(UPrimitiveComponent* TouchedComponent)
{
	WidgetCursorSelectionComponent->SetVisibility(true);
	TriggerWidgetAnim();
}

void AGPRTurretPlatform::OnEndCursorOverPlatformMesh(UPrimitiveComponent* TouchedComponent)
{
	WidgetCursorSelectionComponent->SetVisibility(false);
	StopWidgetAnim();
}

void AGPRTurretPlatform::OnClickedPlatformMesh(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (IsValid(TurretRef))
	{
		// Open upgrade menu ui
		OpenTurretUpgradeShopUI();
	}
	else
	{
		// Open shop menu ui
		PlayerControllerRef->OpenTurretShopUI(this);
	}
}

// Called every frame
void AGPRTurretPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

