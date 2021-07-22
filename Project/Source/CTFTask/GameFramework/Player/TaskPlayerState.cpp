// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskPlayerState.h"

#include "CTFTask/GameFramework/TaskCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void ATaskPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATaskPlayerState, bIsTeamBlue);
	DOREPLIFETIME(ATaskPlayerState, bIsFlagCaptured);
	DOREPLIFETIME(ATaskPlayerState, CurrentHealth);
}

void ATaskPlayerState::BeginPlay()
{
	MaxHealth = 100.0f;
	CurrentHealth = 100;
}


void ATaskPlayerState::OnRep_IsTeamBlue()
{
	UE_LOG(LogTemp, Warning, TEXT("I am from Blue Team =%d"), bIsTeamBlue);
}

void ATaskPlayerState::OnRep_IsFlagCaptured()
{
	CaptureFlagStatusUpdateDelegate.Broadcast(bIsFlagCaptured);
}

void ATaskPlayerState::OnRep_CurrentHealth()
{
	UE_LOG(LogTemp, Warning, TEXT("Heath is replicated"));
	ACTFTaskCharacter* CTFTaskCharacter = Cast<ACTFTaskCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),1)); // because i know only two players that y i ma cheating :)

	if (CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Heath is zero"));
		if (CTFTaskCharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("player found play death animation  "));
			CTFTaskCharacter->PlayDeathAnimation();
		}
	}
}

void ATaskPlayerState::OnHealthUpdate()
{
}
