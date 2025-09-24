// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "GameBoard/GameBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Core/GameStatePM.h"
#include "Components/TilePopulatorComponent.h"
#include "Components/Binding/DelegateBindingCompGameBoard.h"
#include "Components/TileComponent.h"
#include "Components/TilePlanesComponent.h"
#include "Components/ScoreComponent.h"
#include "Logging/StructuredLog.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SparseVolumeTexture/SparseVolumeTexture.h"

// Sets default values
AGameBoard::AGameBoard()
{
	// TileInfoManagerComponent = CreateDefaultSubobject<UTileInfoManagerComponent>(TEXT("Tile Info"));
	// TilePopulatorComponent = CreateDefaultSubobject<UTilePopulatorComponent>(TEXT("TilePopulatorComponent"));
	// TileLineMatchProcessorComponent = CreateDefaultSubobject<UTileLineMatchProcessorComponent>(TEXT("TileLineMatchProcessor"));

	DelegateBindingCompGameBoard = CreateDefaultSubobject<UDelegateBindingCompGameBoard>(TEXT("DelegateBindingCompGameBoard"));
	TileComponent = CreateDefaultSubobject<UTileComponent>(TEXT("TileComponent"));
	ScoreComponent = CreateDefaultSubobject<UScoreComponent>(TEXT("ScoreComponent"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	GameplayTagEmptyTile = FGameplayTag::RequestGameplayTag(FName("EmptyTile"));
}

int32 AGameBoard::GetBoardWidth_Implementation()
{
	return width;
}

int32 AGameBoard::GetBoardHeight_Implementation()
{
	return height;
}



// Called when the game starts or when spawned
void AGameBoard::BeginPlay()
{
	Super::BeginPlay();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int32 index = GetIndexOfArray(x, y);
			FVector PlaneSpawnLocation = GetTileLocationByXandY(x, y);
			TileComponent->TilePlanesComponent->SpawnPlaneAtLocation(PlaneSpawnLocation);
			FGameplayTag GameBoardGameTag = FGameplayTag::RequestGameplayTag(FName("EmptyTile"));
			TileComponent->TileInfoManagerComponent->TileStatuses[index].TileInfo = TileComponent->TileInfoManagerComponent->GetTileInfo(GameBoardGameTag);
			TileComponent->TileInfoManagerComponent->ChangeTileStatus(index, TileComponent->TileInfoManagerComponent->TileStatuses[index]);
			TileComponent->TilePlanesComponent->ChangeTileImage(index, TileComponent->TileInfoManagerComponent->TileStatuses[index]);
		}
	}

	if (AGameStatePM* GameStatePM = Cast<AGameStatePM>(GetWorld()->GetGameState()))
	{
		GameStatePM->GameBoardSendBoardDimensionsDelegate.Broadcast(width, height);
		GameStatePM->GameBoardPopulatedDelegate.Broadcast();
		StartTimer();
	}
}

int32 AGameBoard::GetIndexOfArray(int32 xValue, int32 yValue)
{
	return yValue * width + xValue;
}

FVector AGameBoard::GetTileLocationByXandY(int32 xValue, int32 yValue)
{
	float x = (GridOrigin.X + xValue) * tileMoveAmount;
	float z = (GridOrigin.Z + yValue) * tileMoveAmount;

	return FVector(x, 0.0f, z);
}

// FVector AGameBoard::GetTileLocationByArrayIndex(int32 index)
// {
// 	if (BoardTiles.IsValidIndex(index))
// 	{
// 		return BoardTiles[index]->GetComponentLocation();
// 	}
// 	return BoardTiles[0]->GetComponentLocation();
// }

// void AGameBoard::SpawnPlaneAtLocation(FVector PlaneSpawnLocation)
// {
// 	FTransform PlaneSpawnTransform;
// 	PlaneSpawnTransform.SetLocation(PlaneSpawnLocation);
// 	
// 	UActorComponent* ActorComponent = AddComponentByClass(UStaticMeshComponent::StaticClass(), false, PlaneSpawnTransform, false);
//
// 	if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponent))
// 	{
// 		FRotator Rotator = FRotator(0.0f, 00.0f, -90.0f);
// 		StaticMeshComponent->SetWorldRotation(Rotator);
// 		if (PlaneMesh)
// 		{
// 			StaticMeshComponent->SetStaticMesh(PlaneMesh);
// 		}
// 		StaticMeshComponent->RegisterComponent();
// 		if (BoardTileMaterial)
// 		{
// 			StaticMeshComponent->SetMaterial(0, BoardTileMaterial);
// 		}
// 		BoardTiles.AddUnique(StaticMeshComponent);
// 		StaticMeshComponent->SetupAttachment(GetRootComponent());
// 		
// 	}
// }

// void AGameBoard::CheckforLinesHorizontal()
// {
// 	int32 pointsScored = 0;
//
// 	//Start match at beginning of row
// 	for (int row = 0; row < height; row++)
// 	{
// 		int32 rowIndexStart = row * width;
//
// 		//If the first tile is empty, check if the remaining row is empty.
// 		if (TileComponent->TileInfoManagerComponent->TileStatuses[rowIndexStart].TileInfo->GameplayTag == FGameplayTag::RequestGameplayTag("EmptyTile"))
// 		{
// 			if (IsLineEmptyRow(rowIndexStart))
// 			{
// 				continue;
// 			}
// 		}
// 		
// 		int32 countMatchingTiles = 1;
// 		//Set ptr for window. Left is at the beginning of the row and right is 1 index after.
// 		int32 rowIndexLeft = rowIndexStart;
// 		int32 rowIndexRight = rowIndexLeft + 1;
//
// 		TArray<int32> indexOfPossibledMatches;
// 		indexOfPossibledMatches.Add(rowIndexLeft);
//
// 		//Check matches in the row. Exit loop at end of row
// 		while (rowIndexRight < rowIndexStart + width)
// 		{
// 			const FGameplayTag tagLeft = TileComponent->TileInfoManagerComponent->TileStatuses[rowIndexLeft].TileInfo->GameplayTag;
// 			const FGameplayTag tagRight = TileComponent->TileInfoManagerComponent->TileStatuses[rowIndexRight].TileInfo->GameplayTag;
// 			const FGameplayTag emptyTag = FGameplayTag::RequestGameplayTag(FName("EmptyTile"));
//
// 			if (tagLeft == tagRight && tagLeft != emptyTag)
// 			{
// 				countMatchingTiles++;
// 				indexOfPossibledMatches.Add(rowIndexRight);
// 			}
// 			else
// 			{
// 				if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
// 				{
// 					FMatchGroup MatchGroup;
// 					MatchGroup.indices = indexOfPossibledMatches;
// 					TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
// 					pointsScored += countMatchingTiles * ScoreComponent->pointsPerMatch;
// 				}
// 				countMatchingTiles = 1;
// 				rowIndexLeft = rowIndexRight;
// 				indexOfPossibledMatches.Empty();
// 				indexOfPossibledMatches.Add(rowIndexLeft);
// 			}
// 			rowIndexRight++;
// 		}
//
// 		//At end of row, check matching tile count
// 		if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
// 		{
// 			FMatchGroup MatchGroup;
// 			MatchGroup.indices = indexOfPossibledMatches;
// 			TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
// 			pointsScored += countMatchingTiles * ScoreComponent->pointsPerMatch;
// 		}
// 	}
// 	ScoreComponent->UpdateScore(pointsScored);
// 	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, FString::Printf(TEXT("Scored: %d"), pointsScored));
// }

// bool AGameBoard::IsLineEmptyRow(int32 rowStartIndex)
// {
// 	for (int32 offset = 0; offset < width; offset++)
// 	{
// 		int32 index = rowStartIndex + offset;
// 		
// 		if (TileComponent->TileInfoManagerComponent->TileStatuses.IsValidIndex(index) == false)
// 		{
// 			return true;
// 		}
//
// 		if (TileComponent->TileInfoManagerComponent->TileStatuses[index].TileInfo->GameplayTag != FGameplayTag::RequestGameplayTag(FName("EmptyTile")))
// 		{
// 			return false;
// 		}
// 	}
// 	return true;
// }

// void AGameBoard::CheckforLinesVertical()
// {
// 	int32 pointsScored = 0;
// 	
// 	for (int32 col = 0; col < width; col++)
// 	{
// 		int32 columnIndexBottom = col;
// 		int32 columnIndexUp = columnIndexBottom + width;
// 		int32 countMatchingTiles = 1;
//
// 		if (TileComponent->TileInfoManagerComponent->TileStatuses[columnIndexBottom].TileInfo->GameplayTag == FGameplayTag::RequestGameplayTag("EmptyTile"))
// 		{
// 			if (IsLineEmptyColumn(columnIndexBottom))continue;
// 		}
//
// 		TArray<int32> indexOfPossibledMatches;
// 		indexOfPossibledMatches.Add(columnIndexBottom);
// 		
// 		while (columnIndexUp < TileComponent->TileInfoManagerComponent->TileStatuses.Num() )
// 		{
// 			
// 			if (TileComponent->TileInfoManagerComponent->TileStatuses.IsValidIndex(columnIndexUp) == false)
// 			{
// 				break;
// 			}
// 			FGameplayTag tagBottom = TileComponent->TileInfoManagerComponent->TileStatuses[columnIndexBottom].TileInfo->GameplayTag;
// 			FGameplayTag tagUp = TileComponent->TileInfoManagerComponent->TileStatuses[columnIndexUp].TileInfo->GameplayTag;
// 			FGameplayTag emptyTag = FGameplayTag::RequestGameplayTag("EmptyTile");
// 			
// 			if (tagBottom != emptyTag && tagUp == tagBottom)
// 			{
// 				indexOfPossibledMatches.Add(columnIndexUp);
// 				countMatchingTiles++;
// 			}
// 			else
// 			{
// 				if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
// 				{
// 					FMatchGroup MatchGroup;
// 					MatchGroup.indices = indexOfPossibledMatches;
// 					TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
// 					pointsScored += countMatchingTiles * ScoreComponent->pointsPerMatch;
// 				}
// 				indexOfPossibledMatches.Empty();
// 				countMatchingTiles = 1;
// 				columnIndexBottom = columnIndexUp;
// 				indexOfPossibledMatches.Add(columnIndexBottom);
// 			}
// 			columnIndexUp += width;
// 		}
// 		//At end of row, check matching tile count
// 		if (countMatchingTiles >= ScoreComponent->minimumMatchingForPoint)
// 		{
// 			FMatchGroup MatchGroup;
// 			MatchGroup.indices = indexOfPossibledMatches;
// 			TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.Add(MatchGroup);
// 			pointsScored += countMatchingTiles * ScoreComponent->pointsPerMatch;
// 		}
// 	}
// 	ScoreComponent->UpdateScore(pointsScored);
// 	GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Red, FString::Printf(TEXT("Scored: %d"), pointsScored));
// }

// bool AGameBoard::IsLineEmptyColumn(int32 columnStartIndex)
// {
// 	for (int32 row = 0; row < height; row++)
// 	{
// 		int32 index = columnStartIndex + (row * width);
// 		
// 		if (TileComponent->TileInfoManagerComponent->TileStatuses.IsValidIndex(index) == false)
// 		{
// 			return true;
// 		}
//
// 		if (TileComponent->TileInfoManagerComponent->TileStatuses[index].TileInfo->GameplayTag != FGameplayTag::RequestGameplayTag("EmptyTile"))
// 		{
// 			return false;
// 		}
// 	}
// 	return true;
// }

// void AGameBoard::ProcessMatches()
// {
// 	if (TileLineMatchProcessorComponent->indexOfMatchedTiles.IsEmpty())
// 	{
// 		return;
// 	}
// 	
// 	for (int32 i = 0; i <  indexOfMatchedTiles.Num(); i++)
// 	{
// 		for (int32 j = 0; j < indexOfMatchedTiles[i].indices.Num(); j++)
// 		{
// 			FTileStatus NewTileStatus;
// 			NewTileStatus.TileInfo = GetTileInfo(GameplayTagEmptyTile);
// 			ChangeTileStatus(indexOfMatchedTiles[i].indices[j], NewTileStatus);
// 		}
// 	}
//
// 	 indexOfMatchedTiles.Empty();
// }


// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// void AGameBoard::ChangeTileStatus(int32 IndexTile, FTileStatus NewStatus)
// {
// 	if (NewStatus.TileInfo == nullptr)
// 	{
// 		UE_LOGFMT(LogTemp, Warning, "Missing empty DA in the TileInfo Component. TileInfo withing TileStatus is nullptr for empty tiles");
// 		return;
// 	}
// 	TileStatuses[IndexTile] = NewStatus;
// 	BoardTiles[IndexTile]->SetMaterial(0, NewStatus.TileInfo->Material);
// }

void AGameBoard::SwitchTiles(int32 indexLeft, int32 indexRight)
{

	// const FTransform TransformRight = BoardTiles[indexRight]->GetRelativeTransform();
	// const FTransform TransformLeft = BoardTiles[indexLeft]->GetRelativeTransform();
	// FLatentActionInfo LatentInfoRight;
	// LatentInfoRight.CallbackTarget = this;
	// LatentInfoRight.UUID = __LINE__;
	// LatentInfoRight.Linkage = 0;
	// LatentInfoRight.ExecutionFunction = NAME_None;
	//
	// UKismetSystemLibrary::MoveComponentTo(BoardTiles[indexLeft],TransformRight.GetLocation(), TransformRight.Rotator(),
	// 	false, false, 1.0f, false, EMoveComponentAction::Move, LatentInfoRight);
	//
	// FLatentActionInfo LatentInfoLeft;
	// LatentInfoLeft.CallbackTarget = this;
	// LatentInfoLeft.UUID = __LINE__;
	// LatentInfoLeft.Linkage = 0;
	// LatentInfoLeft.ExecutionFunction = NAME_None;
	//
	// UKismetSystemLibrary::MoveComponentTo(BoardTiles[indexRight],TransformLeft.GetLocation(), TransformLeft.Rotator(),
	// 	false, false, 1.0f, false, EMoveComponentAction::Move, LatentInfoLeft);
	
	FTileStatus LeftStatus = TileComponent->TileInfoManagerComponent->TileStatuses[indexLeft];
	FTileStatus RightStatus = TileComponent->TileInfoManagerComponent->TileStatuses[indexRight];
	
	TileComponent->TileInfoManagerComponent->ChangeTileStatus(indexLeft, RightStatus);
	TileComponent->TileInfoManagerComponent->ChangeTileStatus(indexRight, LeftStatus);

	TileComponent->TilePlanesComponent->ChangeTileImage(indexLeft, RightStatus);
	TileComponent->TilePlanesComponent->ChangeTileImage(indexRight, LeftStatus);

	TileComponent->TileLineMatchProcessorComponent->CheckforLinesHorizontal();
	TileComponent->TileLineMatchProcessorComponent->CheckforLinesVertical();
	// CheckforLinesHorizontal();
	// CheckforLinesVertical();

	if (TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.IsEmpty() == false)
	{
		TileComponent->TileLineMatchProcessorComponent->ProcessMatches();
		TileComponent->TileInfoManagerComponent->CheckTilesBelowAndMove();
	}
}

void AGameBoard::MoveTileRowsUpOneRow()
{

	TArray<FTileStatus> TileStatusesCopy = TileComponent->TileInfoManagerComponent->TileStatuses;
	int32 TotalTiles = TileComponent->TilePlanesComponent->BoardTiles.Num();

	GameOverCheck(TotalTiles, TileStatusesCopy);

	//move rows up. 
	for (int32 i = TotalTiles -1; i >= width; i--)
	{
		TileComponent->TileInfoManagerComponent->ChangeTileStatus(i,TileStatusesCopy[i-width]);
		TileComponent->TilePlanesComponent->ChangeTileImage(i, TileStatusesCopy[i-width]);
	}

	//clear bottom row
	FTileStatus TileStatus;
	TileStatus.TileInfo = TileComponent->TileInfoManagerComponent->GetTileInfo(GameplayTagEmptyTile);
	for (int32 i = 0; i < width; i++)
	{
		TileComponent->TileInfoManagerComponent->ChangeTileStatus(i, TileStatus);
		TileComponent->TilePlanesComponent->ChangeTileImage(i, TileStatus);
	}
}

void AGameBoard::GameOverCheck(int32 TotalTiles, TArray<FTileStatus> TileStatusesCopy)
{
	int32 LastRowStartingIndex = TotalTiles - width;

	for (int32 i = LastRowStartingIndex; i < TotalTiles; i++)
	{
		if (TileStatusesCopy[i].TileInfo && TileStatusesCopy[i].TileInfo->GameplayTag != GameplayTagEmptyTile)
		{
			GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Red, "Game Over");
			UE_LOGFMT(LogTemp, Warning, "Game Over");
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			return;
		}
	}
}

void AGameBoard::PopulateRow(int32 ColumnIndex, TArray<FGameplayTag> GameplayTags)
{
	int32 StartingIndex = width * ColumnIndex;
	int32 EndingIndex = StartingIndex + width - 1;
	int32 tagindex = 0;
	
	for (int32 index = StartingIndex; index <= EndingIndex ; index++)
	{
		FTileStatus NewStatus;
		// NewStatus.TileInfo = GetTileInfo(GameplayTags[tagindex]);
		NewStatus.TileInfo = TileComponent->TileInfoManagerComponent->GetTileInfo(GameplayTags[tagindex]);
		NewStatus.bIsCleared = false;
		NewStatus.bIsOccupied = true;
		TileComponent->TileInfoManagerComponent->ChangeTileStatus(index, NewStatus);
		TileComponent->TilePlanesComponent->ChangeTileImage(index, NewStatus);
		tagindex++;
	}
}

void AGameBoard::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGameBoard::AddNewRowAtBottom, timeNewRowAdd, true);
}

void AGameBoard::AddNewRowAtBottom()
{
	MoveTileRowsUpOneRow();
	TArray<FGameplayTag> NewRowGameplayTags = TileComponent->TilePopulatorComponent->GenerateTileLine();
	PopulateRow(0, NewRowGameplayTags);
}

// UTileInfo* AGameBoard::GetTileInfo(FGameplayTag GameplayTag)
// {
// 	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("EmptyTile")))
// 	{
// 		return TileInfoManagerComponent->TileInfoEmpty;
// 	}
// 	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Sun")))
// 	{
// 		return TileInfoManagerComponent->TileInfoAffectionSun;
// 	}
//
// 	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Toy")))
// 	{
// 		return TileInfoManagerComponent->TileInfoAffectionToy;
// 	}
//
// 	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Plants")))
// 	{
// 		return TileInfoManagerComponent->TileInfoAffectionPlant;
// 	}
//
// 	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Food")))
// 	{
// 		return TileInfoManagerComponent->TileInfoAffectionFood;
// 	}
//
// 	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Bath")))
// 	{
// 		return TileInfoManagerComponent->TileInfoGoalBath;
// 	}
//
// 	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Pet")))
// 	{
// 		return TileInfoManagerComponent->TileInfoGoalPet;
// 	}
//
// 	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Dog")))
// 	{
// 		return TileInfoManagerComponent->TileInfoGoalDog;
// 	}
// 	return TileInfoManagerComponent->TileInfoAffectionSun;
//
// 	
// }

void AGameBoard::GetTileLocation(int32 tileIndex)
{
	FVector tileLocation = TileComponent->TilePlanesComponent->GetTileLocationByArrayIndex(tileIndex);
	DelegateBindingCompGameBoard->GameStatePM->GameBoardOnRequestSendTileLocationDelegate.Broadcast(tileLocation);
}






