// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamFlagBase.h"

#include "TaskCharacter.h"
#include "Net/UnrealNetwork.h"

void ATeamFlagBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ATeamFlagBase,bIsFlagOnTheBase);
}

// Sets default values
ATeamFlagBase::ATeamFlagBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TeamsFlag"));
	BoxComponent->InitBoxExtent(FVector(100.0f,100.0f,100.0f));
	BoxComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionProfileName("Custom");

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagMesh->SetCollisionProfileName("Custom");
	FlagMesh->SetupAttachment(BoxComponent);

}
// Called when the game starts or when spawned
void ATeamFlagBase::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ATeamFlagBase::OnComponentBeginOverlap);
	
}

// void ATeamFlagBase::OnRep_UpdateFlagOnTheBase()
// {
// 	FlagMesh->SetVisibility(false);
// 	UE_LOG(LogTemp,Warning,TEXT("BaseFlagChangeVisibility_Implementation :::%d"),bIsFlagOnTheBase);
//
// }

void ATeamFlagBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("OnComponentBeginOverlap"));
	if(OtherActor != nullptr && bIsFlagOnTheBase)
	{
		UE_LOG(LogTemp,Warning,TEXT("OtherActor"));
		if(ACTFTaskCharacter* CTFTaskCharacter  = Cast<ACTFTaskCharacter>(OtherActor) )
		{
			UE_LOG(LogTemp,Warning,TEXT("CTFTaskCharacter :::"));
			if((CTFTaskCharacter->GetLocalRole() == ROLE_Authority || CTFTaskCharacter->GetLocalRole() == ROLE_SimulatedProxy))
			{
					if(CTFTaskCharacter->TaskPlayerState != nullptr && CTFTaskCharacter->TaskPlayerState->bIsTeamBlue != bIsBlueTeamBase)
					{
						UE_LOG(LogTemp,Warning,TEXT("Player color:::%d::: Base Color:::%d"),CTFTaskCharacter->TaskPlayerState->bIsTeamBlue , bIsBlueTeamBase);

						FlagMesh->SetVisibility(false);
					}
					CTFTaskCharacter->SetFlagVisibility(bIsBlueTeamBase);

			}
			
		}
	}
}
// void ATeamFlagBase::BaseFlagChangeVisibility_Implementation(const bool bIsVisibility)
// {
// //	FlagMesh->SetVisibility(false);
// }
// void ATeamFlagBase::PlayerFlagChangeVisibility_Implementation(ACTFTaskCharacter* TaskCharacter,const bool bIsVisibility)
// {
// 	TaskCharacter->SetFlagVisibility(bIsBlueFlagBase);
//
// }






