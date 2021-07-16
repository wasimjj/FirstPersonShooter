// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamFlag.h"

// Sets default values
ATeamFlag::ATeamFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TeamsFlag"));
	BoxComponent->SetCollisionProfileName("Default");
	BoxComponent->InitBoxExtent(FVector(100.0f,100.0f,100.0f));
	BoxComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeamFlag::BeginPlay()
{
	Super::BeginPlay();
	
}



