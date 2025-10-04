// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ScoreComponent.h"

#include "GameplayTagContainer.h"
#include "Components/TilePopulatorComponent.h"
#include "Core/GameStatePM.h"
#include "Core/PlayerStatePM.h"
#include "GameBoard/GameBoard.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UScoreComponent::UScoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UScoreComponent::UpdateScore(int32 pointsScored, FGameplayTag GameplayTag)
{
	if (APlayerState* PlayerState = UGameplayStatics::GetPlayerState(GetWorld(), 0))
	{
		if (APlayerStatePM* PlayerStatePM = Cast<APlayerStatePM>(PlayerState))
		{
			// score += pointsScored;
			// GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Green, FString::Printf(TEXT("Scored: %d"), score));
			int32 signedPointsScored = 0;
			int32 currentScore = PlayerStatePM->GetScore();
			if (currentScore <= totalAffectionNeeded)
			{
				if (GameplayTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Affection"))))
				{
					signedPointsScored = pointsScored;
				}
				else
				{
					signedPointsScored = pointsScored * -1;
				}
			}
			else
			{
				signedPointsScored = pointsScored;

				if (isChanceOfGoalTileUpdated == false)
				{
					SetIsChanceOfGoalTileUpdated(true);
					if (AGameBoard* GameBoard = Cast<AGameBoard>(GetOwner()))
					{
						if (UTileComponent* TileComponent = GameBoard->GetComponentByClass<UTileComponent>())
						{
							TileComponent->TilePopulatorComponent->OnAffectionFullChangeChanceOfGoalTile();
						}
					}
				}

				if (GameplayTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Goal"))))
				{
					signedPointsScored = pointsScored;

					if (AGameStatePM* GameStatePM = Cast<AGameStatePM>(GetWorld()->GetGameState()))
					{
						GameStatePM->GameStateLevelCompletedDelegate.Broadcast();
					}
				}
			}

			PlayerStatePM->ChangeScore(signedPointsScored);
		}
	}
}

void UScoreComponent::SetTotalAffectionNeeded(int32 InTotalAffectionNeeded)
{
	totalAffectionNeeded = InTotalAffectionNeeded;
}

void UScoreComponent::SetIsChanceOfGoalTileUpdated(bool InIsChangeOfGoalTileUpdated)
{
	isChanceOfGoalTileUpdated = InIsChangeOfGoalTileUpdated;
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

