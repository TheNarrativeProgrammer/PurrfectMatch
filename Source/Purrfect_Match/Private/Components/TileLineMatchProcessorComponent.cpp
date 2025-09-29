// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/TileLineMatchProcessorComponent.h"

#include "Components/ScoreComponent.h"
#include "Components/TileComponent.h"
#include "Components/TilePlanesComponent.h"
#include "Core/PlayerStatePM.h"
#include "Data/TileStatus.h"
#include "GameBoard/GameBoard.h"
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
	
	for (int32 i = 0; i < indexOfMatchedTiles.Num(); i++)
	{
		for (int32 j = 0; j < indexOfMatchedTiles[i].indices.Num(); j++)
		{
			FTileStatus NewTileStatus;
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
		}
	}

	indexOfMatchedTiles.Empty();
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

					//Check matches in the row. Exit loop at end of row
					while (rowIndexRight < rowIndexStart + IBoardable::Execute_GetBoardWidth(ActorOwner))
					{
						const FGameplayTag tagLeft = TileComponent->TileInfoManagerComponent->TileStatuses[rowIndexLeft].TileInfo->GameplayTag;
						const FGameplayTag tagRight = TileComponent->TileInfoManagerComponent->TileStatuses[rowIndexRight].TileInfo->GameplayTag;
						const FGameplayTag emptyTag = FGameplayTag::RequestGameplayTag(FName("EmptyTile"));

						if (tagLeft == tagRight && tagLeft != emptyTag)
						{
							countMatchingTiles++;
							indexOfPossibledMatches.Add(rowIndexRight);
						}
						else
						{
							if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
							{
								FMatchGroup MatchGroup;
								MatchGroup.indices = indexOfPossibledMatches;
								TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
								pointsScored += countMatchingTiles * ScoreComponent->pointsPerMatch;
							}
							countMatchingTiles = 1;
							rowIndexLeft = rowIndexRight;
							indexOfPossibledMatches.Empty();
							indexOfPossibledMatches.Add(rowIndexLeft);
						}
						rowIndexRight++;
					}

					//At end of row, check matching tile count
					if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
					{
						FMatchGroup MatchGroup;
						MatchGroup.indices = indexOfPossibledMatches;
						TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
						pointsScored += countMatchingTiles * ScoreComponent->pointsPerMatch;
					}
				}
				ScoreComponent->UpdateScore(pointsScored);
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
					
					while (columnIndexUp < TileComponent->TileInfoManagerComponent->TileStatuses.Num() )
					{
						
						if (TileComponent->TileInfoManagerComponent->TileStatuses.IsValidIndex(columnIndexUp) == false)
						{
							break;
						}
						FGameplayTag tagBottom = TileComponent->TileInfoManagerComponent->TileStatuses[columnIndexBottom].TileInfo->GameplayTag;
						FGameplayTag tagUp = TileComponent->TileInfoManagerComponent->TileStatuses[columnIndexUp].TileInfo->GameplayTag;
						FGameplayTag emptyTag = FGameplayTag::RequestGameplayTag("EmptyTile");
						
						if (tagBottom != emptyTag && tagUp == tagBottom)
						{
							indexOfPossibledMatches.Add(columnIndexUp);
							countMatchingTiles++;
						}
						else
						{
							if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
							{
								FMatchGroup MatchGroup;
								MatchGroup.indices = indexOfPossibledMatches;
								TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
								pointsScored += countMatchingTiles * ScoreComponent->pointsPerMatch;
							}
							indexOfPossibledMatches.Empty();
							countMatchingTiles = 1;
							columnIndexBottom = columnIndexUp;
							indexOfPossibledMatches.Add(columnIndexBottom);
						}
						columnIndexUp += IBoardable::Execute_GetBoardWidth(ActorOwner);
					}
					//At end of row, check matching tile count
					if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
					{
						FMatchGroup MatchGroup;
						MatchGroup.indices = indexOfPossibledMatches;
						TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
						pointsScored += countMatchingTiles * ScoreComponent->pointsPerMatch;
					}
				}
				ScoreComponent->UpdateScore(pointsScored);
			}
		}
	}
	
	GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Red, FString::Printf(TEXT("Scored: %d"), pointsScored));
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


// Called every frame
void UTileLineMatchProcessorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

