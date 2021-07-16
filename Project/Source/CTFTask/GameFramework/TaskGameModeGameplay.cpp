// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameModeGameplay.h"
#include "TaskHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ATaskGameModeGameplay::ATaskGameModeGameplay() 
{
	HUDClass = ATaskHUD::StaticClass();
	
}

void ATaskGameModeGameplay::BeginPlay()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}
