// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskGameMode.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "TaskGameModeMainMenu.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly,Category="Server Info")
	TArray<FString> ServerList;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSearchServerCompletedDelegate,FServerInfo, ServerInfo);
UCLASS()
class CTFTASK_API ATaskGameModeMainMenu : public ATaskGameMode
{
	GENERATED_BODY()
public:
	ATaskGameModeMainMenu();
	IOnlineSessionPtr OnlineSessionPtr;
	TSharedPtr<FOnlineSessionSearch> OnlineSessionSearch;
	UFUNCTION(BlueprintCallable, Category="Session")
	void OnSessionComplete(FName ServerName, bool Success);
	UFUNCTION(BlueprintCallable, Category="Session")
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION(BlueprintCallable, Category= "Session")
	void CreateServer();
	UFUNCTION(BlueprintCallable, Category="Session")
	void JoinServer();
	UFUNCTION(BlueprintCallable, Category="Session")
	void FindServer();
	UFUNCTION(BlueprintCallable , Category="Session")
	TArray<FString> GetServers();
	public:
	UPROPERTY(BlueprintAssignable , Category="Session List")
	FOnSearchServerCompletedDelegate OnSearchServerCompletedDelegate;
};

