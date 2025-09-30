// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ScoreComponent.h"

#include "Core/PlayerStatePM.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UScoreComponent::UScoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UScoreComponent::UpdateScore(int32 pointsScored)
{
	
	if (APlayerState* PlayerState = UGameplayStatics::GetPlayerState(GetWorld(), 0))
	{
		if (APlayerStatePM* PlayerStatePM = Cast<APlayerStatePM>(PlayerState))
		{
			PlayerStatePM->ChangeScore(pointsScored);
		}
	}
	// score += pointsScored;
	// GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Green, FString::Printf(TEXT("Scored: %d"), score));
}

// Called when the game starts
void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UScoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

