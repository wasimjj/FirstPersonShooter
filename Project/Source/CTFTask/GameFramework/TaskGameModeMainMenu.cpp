// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameModeMainMenu.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

ATaskGameModeMainMenu::ATaskGameModeMainMenu()
{
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		OnlineSessionPtr = OnlineSubsystem->GetSessionInterface();
		if (OnlineSessionPtr.IsValid())
		{
			OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddUObject(
				this, &ATaskGameModeMainMenu::OnSessionComplete);
			OnlineSessionPtr->OnFindSessionsCompleteDelegates.AddUObject(
				this, &ATaskGameModeMainMenu::OnFindSessionsComplete);
			OnlineSessionPtr->OnJoinSessionCompleteDelegates.AddUObject(
				this, &ATaskGameModeMainMenu::OnJoinSessionComplete);
		}
	}
}

void ATaskGameModeMainMenu::OnSessionComplete(FName ServerName, bool Success)
{
	if (Success && GetWorld() != nullptr)
	{
		GLog->Log("Session Created");
		GetWorld()->ServerTravel("/Game/Maps/gameplay?listen");
	}
}

void ATaskGameModeMainMenu::OnFindSessionsComplete(bool bSuccess)
{
	if (bSuccess && OnlineSessionSearch != nullptr)
	{
		TArray<FOnlineSessionSearchResult> OnlineSessionSearchResult = OnlineSessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("Total Session Found:::%d ::: "), OnlineSessionSearchResult.Num());
		if (OnlineSessionSearchResult.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Total Session Found:::%s ::: "),
			       *OnlineSessionSearchResult[0].Session.GetSessionIdStr());
			OnSearchServerCompletedDelegate.Broadcast(FServerInfo{TArray<FString>{"Server1","Server2"}});
		}
	}
}

void ATaskGameModeMainMenu::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString Address = "";
		OnlineSessionPtr->GetResolvedConnectString(SessionName, Address);
		if (!Address.IsEmpty())
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
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
	OnlineSessionPtr->CreateSession(0, FName("Test Session"), OnlineSessionSettings);
}

void ATaskGameModeMainMenu::JoinServer()
{
	if (OnlineSessionPtr != nullptr && OnlineSessionSearch != nullptr && OnlineSessionSearch->SearchResults.Num() > 0)
	{
		OnlineSessionPtr->JoinSession(0, "Test Session", OnlineSessionSearch->SearchResults[0]);
	}
}

void ATaskGameModeMainMenu::FindServer()
{
	OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch());
	OnlineSessionSearch->bIsLanQuery = true;
	OnlineSessionSearch->MaxSearchResults = 20;
	OnlineSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	OnlineSessionPtr->FindSessions(0, OnlineSessionSearch.ToSharedRef());
}

TArray<FString> ATaskGameModeMainMenu::GetServers()
{
	if(OnlineSessionSearch!=nullptr)
	{
		return TArray<FString>{"Server 1 ", "Server2"};
	}
	return TArray<FString>{"Server 1 ", "Server2"};
}
