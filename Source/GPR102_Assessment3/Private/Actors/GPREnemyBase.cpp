// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GPREnemyBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Actors/GPRHeadquartersBase.h"
#include "Components/ProgressBar.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/GPRPlayerController.h"
#include "Core/GPRPlayerPawnBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Widgets/GPREnemyHealthBarWidget.h"

// Sets default values
AGPREnemyBase::AGPREnemyBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(RootComponent);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(EnemyMesh);
}

// Called when the game starts or when spawned
void AGPREnemyBase::BeginPlay()
{
	Super::BeginPlay();

	SetupFunctionBindings(); // Sets up function bindings	
	SetupPlayerControllerReference(); // Sets up the player controller reference
	SetupEnemyHealthBarWidgetRef(); // Sets up a reference to the enemy health bar widget currently being used by this enemy
}

// Called every frame
void AGPREnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Updates the enemy health bar using an FInterp to smoothly update it's percentage
	UpdateEnemyHealthBar();
}

void AGPREnemyBase::SetupEnemyHealthBarWidgetRef()
{
	// Casts from the widget health bar components currently used user widget
	if (TObjectPtr<UGPREnemyHealthBarWidget> LocalWidgetRef = Cast<UGPREnemyHealthBarWidget>(HealthBarWidget->GetWidget()))
	{
		EnemyHealthBarWidgetRef = LocalWidgetRef;
	}
}

void AGPREnemyBase::UpdateEnemyHealthBar()
{
	// Null ptr check
	if (IsValid(EnemyHealthBarWidgetRef) && IsValid(EnemyHealthBarWidgetRef->HealthProgressBar))
	{
		// Declares a local ptr variable to the enemies widget health progress bar component
		TObjectPtr<UProgressBar> LocalProgressBar = EnemyHealthBarWidgetRef->HealthProgressBar;

		// Uses FInterp to smoothly interp the enemies health bar when damaged
		const float LocalCurrentHealthPercentage = LocalProgressBar->GetPercent();
		const float LocalTargetHealthPercentage = CurrentHealth / MaxHealth;
		const float LocalProgress = FMath::FInterpTo(LocalCurrentHealthPercentage, LocalTargetHealthPercentage, GetWorld()->GetDeltaSeconds(), 15.f);

		// Finally sets the enemies health progress bar percentage
		LocalProgressBar->SetPercent(LocalProgress);
	}
}

void AGPREnemyBase::MoveEnemyAlongSpline(const float Progress)
{
	const float LocalDistance = FMath::Lerp(0.f, SplineRef->GetSplineLength(), Progress);
	FVector LocalNewLocation = SplineRef->GetLocationAtDistanceAlongSpline(LocalDistance, ESplineCoordinateSpace::World);
	FRotator LocalNewRotation = SplineRef->GetRotationAtDistanceAlongSpline(LocalDistance, ESplineCoordinateSpace::World);

	// Sets the actors new location & rotation based on the progress along the spline
	SetActorLocationAndRotation(LocalNewLocation, LocalNewRotation);
}

void AGPREnemyBase::OnDestroyedEnemyBase(AActor* DestroyedActor)
{
	// Will only award credits to the player if their turrets projectile destroyed it not the headquarters
	if (!Cast<AGPRHeadquartersBase>(GetInstigator()))
	{
		// Null ptr check
		if (IsValid(PlayerControllerRef) && IsValid(PlayerControllerRef->PlayerPawnBaseRef))
		{
			// When this enemy instance dies it will add 2 credits to the players credit score
			PlayerControllerRef->PlayerPawnBaseRef->UpdateCreditPoints(2);
		}
	}

	// Null ptr check
	if (IsValid(EnemyDeathVFX))
	{
		// Spawns a niagara system at the location of this actor when destroyed
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EnemyDeathVFX,
													   GetActorLocation(), FRotator::ZeroRotator,
													   FVector::OneVector, true,
													   true, ENCPoolMethod::None,
													   true);
	}

	// Null ptr check
	if (IsValid(EnemyDeathSFX))
	{
		// Plays a death sound effect at this actors location
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyDeathSFX, GetActorLocation());
	}
}

void AGPREnemyBase::AnyDamageEnemyBase(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	// Checks if the enemies health after taking damage will be more than 0
	if (CurrentHealth - Damage > 0.f)
	{
		/* If the enemy will have more than 0 health after taking damage
		 * then the enemies current health will be modified */
		CurrentHealth -= Damage;
	}
	else
	{
		// If the enemy has 0 health left than it will be destoryed
		this->Destroy();
	}
}

void AGPREnemyBase::SetupPlayerControllerReference()
{
	// Casts from the worlds first found player controller to the currently used player controller
	if (TObjectPtr<AGPRPlayerController> LocalPlayerController = Cast<AGPRPlayerController>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		// Stores a reference to the player controller that was cast to
		PlayerControllerRef = LocalPlayerController;
	}
}

void AGPREnemyBase::SetupFunctionBindings()
{
	OnDestroyed.AddDynamic(this, &ThisClass::OnDestroyedEnemyBase);
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::AnyDamageEnemyBase);
}
