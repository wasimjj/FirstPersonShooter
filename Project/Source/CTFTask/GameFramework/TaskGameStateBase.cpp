// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameStateBase.h"

#include "Net/UnrealNetwork.h"

void ATaskGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATaskGameStateBase,BlueTeamScore);
	DOREPLIFETIME(ATaskGameStateBase,RedTeamScore);
}

void ATaskGameStateBase::OnRep_OnRedTeamScore()
{
}

void ATaskGameStateBase::OnRep_OnBlueTeamScore()
{
}
