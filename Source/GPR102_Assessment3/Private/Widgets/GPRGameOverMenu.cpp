// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GPRGameOverMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGPRGameOverMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Sets up all function bindings
	SetupFunctionBindings();	
}

void UGPRGameOverMenu::SetupFunctionBindings()
{
	RestartButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedRestartButton);
	MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMainMenuButton);
}

void UGPRGameOverMenu::OnClickedRestartButton()
{
	/* Restarts the level by getting the currently opened
	 * level name & using it to open the same level
	 * This restarts the level and resets all data */
	UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()), false);
}

void UGPRGameOverMenu::OnClickedMainMenuButton()
{
	UGameplayStatics::OpenLevel(this, FName("L_MainMenu"), false);
}
