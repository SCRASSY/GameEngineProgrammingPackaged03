// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GPRMainMenu.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UGPRMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Sets up button function bindings
	SetupFunctionBindings();
}

void UGPRMainMenu::SetupFunctionBindings()
{
	StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedStartGameButton);
	QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedExitGameButton);
}

void UGPRMainMenu::OnClickedStartGameButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Scifi_ModernCity"), false);
}

void UGPRMainMenu::OnClickedExitGameButton()
{
	// Null ptr check
	if (IsValid(GetWorld()))
	{
		// Get the world's first player controller & if valid will quit the game
		if (TObjectPtr<APlayerController> LocalPlayerController = GetWorld()->GetFirstPlayerController())
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), LocalPlayerController, EQuitPreference::Quit, false);
		}
	}
}
