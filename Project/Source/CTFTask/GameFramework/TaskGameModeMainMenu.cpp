// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameModeMainMenu.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
ATaskGameModeMainMenu::ATaskGameModeMainMenu()
{
		if(IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
		{
			OnlineSessionPtr = OnlineSubsystem->GetSessionInterface();
			if(OnlineSessionPtr.IsValid())
			{
				OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this,&ATaskGameModeMainMenu::OnSessionComplete);
			}
		}
}

void ATaskGameModeMainMenu::OnSessionComplete(FName ServerName, bool Success)
{
}

void ATaskGameModeMainMenu::CreateServer()
{
	FOnlineSessionSettings OnlineSessionSettings;
	OnlineSessionSettings.bIsDedicated = false;
	OnlineSessionSettings.bIsLANMatch = true;
	OnlineSessionSettings.bAllowJoinInProgress = true;
	OnlineSessionSettings.NumPublicConnections = 2;
	OnlineSessionSettings.bUsesPresence = true;
	OnlineSessionSettings.bShouldAdvertise = true;
	OnlineSessionPtr->CreateSession(0,FName("Test Session"),OnlineSessionSettings);
}

void ATaskGameModeMainMenu::JoinServer()
{
}
