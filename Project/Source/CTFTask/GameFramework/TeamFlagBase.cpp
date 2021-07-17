// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamFlagBase.h"

#include "TaskCharacter.h"

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

void ATeamFlagBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != nullptr && bIsFlagOnTheBase)
	{
		if(ACTFTaskCharacter* CTFTaskCharacter  = Cast<ACTFTaskCharacter>(OtherActor) )
		{
			if(CTFTaskCharacter->TaskPlayerState != nullptr)
			{
				UE_LOG(LogTemp,Warning,TEXT("Player color:::%d::: Base Color:::%d"),CTFTaskCharacter->TaskPlayerState->bIsTeamBlue , bIsBlueFlagBase);
				if(CTFTaskCharacter->TaskPlayerState->bIsTeamBlue != bIsBlueFlagBase)
				{
					//if(GetLocalRole() == ROLE_Authority)
					{
						FlagMesh->SetVisibility(false);
						CTFTaskCharacter->SetFlagVisibility(bIsBlueFlagBase);
					}
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void ATeamFlagBase::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ATeamFlagBase::OnComponentBeginOverlap);

	
}



