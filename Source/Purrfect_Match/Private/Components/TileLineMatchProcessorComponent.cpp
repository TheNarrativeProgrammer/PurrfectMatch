// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/TileLineMatchProcessorComponent.h"

#include "Components/ScoreComponent.h"
#include "Components/TileComponent.h"
#include "Components/TilePlanesComponent.h"
#include "Core/PlayerStatePM.h"
#include "Data/TileStatus.h"
#include "GameBoard/GameBoard.h"
#include "Logging/StructuredLog.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTileLineMatchProcessorComponent::UTileLineMatchProcessorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	GameplayTagEmptyTile = FGameplayTag::RequestGameplayTag(FName("EmptyTile"));
	// ...
}


// Called when the game starts
void UTileLineMatchProcessorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTileLineMatchProcessorComponent::ProcessMatches()
{
	if (indexOfMatchedTiles.IsEmpty())
	{
		return;
	}
	int32 arrayLength = indexOfMatchedTiles.Num() - 1;
	
	for (int32 i = arrayLength; i >= 0; i--)
	{
		for (int32 j = 0; j < indexOfMatchedTiles[i].indices.Num(); j++)
		{
			FTileStatus NewTileStatus;
			ProcessPoints(indexOfMatchedTiles[i].GameplayTagGoalOrAffection, indexOfMatchedTiles[i].points);
			if (UTileComponent* TileComponent = Cast<UTileComponent>(GetOwner()->GetComponentByClass(UTileComponent::StaticClass())))
			{
				if (AGameBoard* GameBoardOwner = Cast<AGameBoard>(GetOwner()))
				{
					if (UTileInfoManagerComponent* TileInfoManagerComponent = GameBoardOwner->GetComponentByClass<UTileInfoManagerComponent>())
					{
						NewTileStatus.TileInfo = TileComponent->TileInfoManagerComponent->GetTileInfo(GameplayTagEmptyTile);
						TileInfoManagerComponent->ChangeTileStatus(indexOfMatchedTiles[i].indices[j], NewTileStatus);

						if (UTilePlanesComponent* TilePlanesComponent = GameBoardOwner->GetComponentByClass<UTilePlanesComponent>())
						{
							TilePlanesComponent->ChangeTileImage(indexOfMatchedTiles[i].indices[j], NewTileStatus);
						}
					}
				}
			}

			if (AGameBoard* GameBoardOwner = Cast<AGameBoard>(GetOwner()))
			{
				if (UTilePlanesComponent* TilePlanesComponent =  GameBoardOwner->GetComponentByClass<UTilePlanesComponent>())
				{
					TilePlanesComponent->SpawnScoreMaterialPlane(indexOfMatchedTiles[i].indices[j], indexOfMatchedTiles[i].GameplayTagGoalOrAffection);
				}
			}
		}
 		indexOfMatchedTiles.RemoveAt(i);
	}

	//indexOfMatchedTiles.Empty();
}

void UTileLineMatchProcessorComponent::ProcessPoints(FGameplayTag GameplayTag, int32 pointsScored)
{
	UE_LOGFMT(LogTemp, Warning, "Tag {0}", GameplayTag.ToString());
	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		if (UScoreComponent* ScoreComponent = ActorOwner->GetComponentByClass<UScoreComponent>())
		{
			ScoreComponent->UpdateScore(pointsScored);
			if (GameplayTag.IsValid() && GameplayTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Affection"))))
			{
				//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "ProcessPoints");
			}
		}
	}
}

void UTileLineMatchProcessorComponent::CheckforLinesHorizontal()
{
	int32 pointsScored = 0;

	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		if (UTileComponent* TileComponent = ActorOwner->GetComponentByClass<UTileComponent>())
		{
			if (UScoreComponent* ScoreComponent = ActorOwner->GetComponentByClass<UScoreComponent>())
			{
				//Start match at beginning of row
				for (int row = 0; row < IBoardable::Execute_GetBoardHeight(ActorOwner); row++)
				{
					int32 rowIndexStart = row * IBoardable::Execute_GetBoardWidth(ActorOwner);

					//If the first tile is empty, check if the remaining row is empty.
					if (TileComponent->TileInfoManagerComponent->TileStatuses[rowIndexStart].TileInfo->GameplayTag == FGameplayTag::RequestGameplayTag("EmptyTile"))
					{
						if (IsLineEmptyRow(rowIndexStart))
						{
							continue;
						}
					}
				
					int32 countMatchingTiles = 1;
					//Set ptr for window. Left is at the beginning of the row and right is 1 index after.
					int32 rowIndexLeft = rowIndexStart;
					int32 rowIndexRight = rowIndexLeft + 1;

					TArray<int32> indexOfPossibledMatches;
					indexOfPossibledMatches.Add(rowIndexLeft);
					FGameplayTag tagMatchStart;

					//Check matches in the row. Exit loop at end of row
					while (rowIndexRight < rowIndexStart + IBoardable::Execute_GetBoardWidth(ActorOwner))
					{
						const FGameplayTag tagLeft = TileComponent->TileInfoManagerComponent->TileStatuses[rowIndexLeft].TileInfo->GameplayTag;
						const FGameplayTag tagRight = TileComponent->TileInfoManagerComponent->TileStatuses[rowIndexRight].TileInfo->GameplayTag;
						const FGameplayTag emptyTag = FGameplayTag::RequestGameplayTag(FName("EmptyTile"));
						tagMatchStart = tagLeft;

						if (tagLeft == tagRight && tagLeft != emptyTag)
						{
							countMatchingTiles++;
							indexOfPossibledMatches.Add(rowIndexRight);
						}
						else
						{
							if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
							{
								ProcessMatchGroup(pointsScored, tagMatchStart, indexOfPossibledMatches, TileComponent, ScoreComponent, countMatchingTiles);
							}

							ResetCountIndexAndArray(countMatchingTiles, rowIndexLeft, rowIndexRight, indexOfPossibledMatches);
						}
						rowIndexRight++;
					}

					//At end of row, check matching tile count
					if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
					{
						ProcessMatchGroup(pointsScored, tagMatchStart, indexOfPossibledMatches, TileComponent, ScoreComponent, countMatchingTiles);
					}
				}
				// ScoreComponent->UpdateScore(pointsScored);
			}
		}
	}
}

bool UTileLineMatchProcessorComponent::IsLineEmptyRow(int32 rowStartIndex)
{
	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		if (UTileComponent* TileComponent = ActorOwner->GetComponentByClass<UTileComponent>())
		{
			for (int32 offset = 0; offset < IBoardable::Execute_GetBoardWidth(ActorOwner); offset++)
			{
				int32 index = rowStartIndex + offset;
		
				if (TileComponent->TileInfoManagerComponent->TileStatuses.IsValidIndex(index) == false)
				{
					return true;
				}

				if (TileComponent->TileInfoManagerComponent->TileStatuses[index].TileInfo->GameplayTag != FGameplayTag::RequestGameplayTag(FName("EmptyTile")))
				{
					return false;
				}
			}
			return true;
		}
	}
	return true;
}

void UTileLineMatchProcessorComponent::CheckforLinesVertical()
{
	int32 pointsScored = 0;
	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		if (UTileComponent* TileComponent = ActorOwner->GetComponentByClass<UTileComponent>())
		{
			if (UScoreComponent* ScoreComponent = ActorOwner->GetComponentByClass<UScoreComponent>())
			{
				for (int32 col = 0; col < IBoardable::Execute_GetBoardWidth(ActorOwner); col++)
				{
					int32 columnIndexBottom = col;
					int32 columnIndexUp = columnIndexBottom + IBoardable::Execute_GetBoardWidth(ActorOwner);
					int32 countMatchingTiles = 1;

					if (TileComponent->TileInfoManagerComponent->TileStatuses[columnIndexBottom].TileInfo->GameplayTag == FGameplayTag::RequestGameplayTag("EmptyTile"))
					{
						if (IsLineEmptyColumn(columnIndexBottom))continue;
					}

					TArray<int32> indexOfPossibledMatches;
					indexOfPossibledMatches.Add(columnIndexBottom);
					FGameplayTag tagMatchStart;
					
					while (columnIndexUp < TileComponent->TileInfoManagerComponent->TileStatuses.Num() )
					{
						
						if (TileComponent->TileInfoManagerComponent->TileStatuses.IsValidIndex(columnIndexUp) == false)
						{
							break;
						}
						FGameplayTag tagBottom = TileComponent->TileInfoManagerComponent->TileStatuses[columnIndexBottom].TileInfo->GameplayTag;
						FGameplayTag tagUp = TileComponent->TileInfoManagerComponent->TileStatuses[columnIndexUp].TileInfo->GameplayTag;
						FGameplayTag emptyTag = FGameplayTag::RequestGameplayTag("EmptyTile");
						tagMatchStart = tagBottom;
						
						if (tagBottom != emptyTag && tagUp == tagBottom)
						{
							indexOfPossibledMatches.Add(columnIndexUp);
							countMatchingTiles++;
						}
						else
						{
							if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
							{
								ProcessMatchGroup(pointsScored, tagMatchStart, indexOfPossibledMatches, TileComponent, ScoreComponent, countMatchingTiles);
							}
							ResetCountIndexAndArray(countMatchingTiles, columnIndexBottom, columnIndexUp, indexOfPossibledMatches);
						}
						columnIndexUp += IBoardable::Execute_GetBoardWidth(ActorOwner);
					}
					//At end of row, check matching tile count
					if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
					{
						ProcessMatchGroup(pointsScored, tagMatchStart, indexOfPossibledMatches, TileComponent, ScoreComponent, countMatchingTiles);
					}
				}
				//ScoreComponent->UpdateScore(pointsScored);
			}
		}
	}
	
}

bool UTileLineMatchProcessorComponent::IsLineEmptyColumn(int32 columnStartIndex)
{
	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		if (UTileComponent* TileComponent = ActorOwner->GetComponentByClass<UTileComponent>())
		{
			for (int32 row = 0; row < IBoardable::Execute_GetBoardHeight(ActorOwner); row++)
			{
				int32 index = columnStartIndex + (row * IBoardable::Execute_GetBoardWidth(ActorOwner));
		
				if (TileComponent->TileInfoManagerComponent->TileStatuses.IsValidIndex(index) == false)
				{
					return true;
				}

				if (TileComponent->TileInfoManagerComponent->TileStatuses[index].TileInfo->GameplayTag != FGameplayTag::RequestGameplayTag("EmptyTile"))
				{
					return false;
				}
			}
			return true;
		}
	}
	return true;
}

void UTileLineMatchProcessorComponent::ProcessMatchGroup(int& PointsScored, FGameplayTag TagLeft, TArray<int32> indexOfPossibledMatches, UTileComponent* TileComponent, UScoreComponent* ScoreComponent, int CountMatchingTiles)
{
	FMatchGroup MatchGroup;
	MatchGroup.indices = indexOfPossibledMatches;
	int32 pointsScored = 0;
	if (TagLeft.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Affection"))))
	{
		MatchGroup.GameplayTagGoalOrAffection = FGameplayTag::RequestGameplayTag(FName("Affection"));
		//PointsScored += CountMatchingTiles * ScoreComponent->pointsPerMatch;
		pointsScored = CountMatchingTiles * ScoreComponent->pointsPerMatch;
	}
	else
	{
		MatchGroup.GameplayTagGoalOrAffection = FGameplayTag::RequestGameplayTag(FName("Goal"));
		//PointsScored += CountMatchingTiles * ScoreComponent->pointsPerMatch;
		pointsScored = (CountMatchingTiles * ScoreComponent->pointsPerMatch) * -1;
	}
	
	
	MatchGroup.points = pointsScored;
	TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
	
}

void UTileLineMatchProcessorComponent::ResetCountIndexAndArray(int32& CountMatchingTiles,
	int32& SlidingWindowStartIndex, int32 NewWindowStartIndex, TArray<int32>& IndexOfPossibledMatches)
{
	CountMatchingTiles = 1;
	SlidingWindowStartIndex = NewWindowStartIndex;
	IndexOfPossibledMatches.Empty();
	IndexOfPossibledMatches.Add(SlidingWindowStartIndex);
}


// Called every frame
void UTileLineMatchProcessorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

