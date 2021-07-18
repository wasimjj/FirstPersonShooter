// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskPlayerState.h"

#include "Net/UnrealNetwork.h"


void ATaskPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATaskPlayerState,bIsTeamBlue);
}

void ATaskPlayerState::OnRep_Score()
{
	UE_LOG(LogTemp,Warning, TEXT("ATaskPlayerState::OnRep_Score:::%d"),GetScore());
	Super::OnRep_Score();
	OnScoreUpdateDelegate.Broadcast(GetScore());
}

void ATaskPlayerState::OnRep_IsTeamBlue()
{
	UE_LOG(LogTemp,Warning,TEXT("I am from Blue Team =%d"),bIsTeamBlue);
}
