// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskGameMode.h"
#include "CTFTask/Engine/TaskGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Player/TaskPlayerState.h"

#include "TaskGameModeMainMenu.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly,Category="Server Info")
	FString ServerName;
	UPROPERTY(BlueprintReadOnly,Category="Server Info")
	int SessionIndex;
	UPROPERTY(BlueprintReadOnly,Category="Server Info")
	int MaxPlayers;
	UPROPERTY(BlueprintReadOnly,Category="Server Info")
	int JoinedPlayers;
	
};
USTRUCT(BlueprintType)
struct FServerInfoList
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly,Category="Server Info")
	TArray<FServerInfo> ServerList;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSearchServerCompletedDelegate,FServerInfoList, ServerInfoList);
UCLASS()
class CTFTASK_API ATaskGameModeMainMenu : public ATaskGameMode
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	ATaskGameModeMainMenu();
	IOnlineSessionPtr OnlineSessionPtr;
	TSharedPtr<FOnlineSessionSearch> OnlineSessionSearch;
	UFUNCTION(BlueprintCallable, Category="Session")
	void OnSessionComplete(FName ServerName, bool Success);
	UFUNCTION(BlueprintCallable, Category="Session")
	void OnFindSessionsComplete(const bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION(BlueprintCallable, Category= "Session")
	void CreateServer(const FString ServerName);
	UFUNCTION(BlueprintCallable, Category="Session")
	void JoinServer(const FName ServerName ,const int SessionIndex);
	UFUNCTION(BlueprintCallable, Category="Session")
	void FindServer();
	UFUNCTION(BlueprintCallable , Category="Session")
	TArray<FString> GetServers();
	public:
	UPROPERTY(BlueprintAssignable , Category="Session List")
	FOnSearchServerCompletedDelegate OnSearchServerCompletedDelegate;
	UPROPERTY()
	UTaskGameInstance* TaskGameInstance;
};

