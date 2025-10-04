// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "GameBoard/GameBoard.h"

#include "Components/LevelDataManagerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/GameStatePM.h"
#include "Components/TilePopulatorComponent.h"
#include "Components/Binding/DelegateBindingCompGameBoard.h"
#include "Components/TileComponent.h"
#include "Components/TilePlanesComponent.h"
#include "Components/ScoreComponent.h"
#include "Core/PlayerStatePM.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
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
	LevelDataManagerComponent = CreateDefaultSubobject<ULevelDataManagerComponent>(TEXT("LevelDataManagerComponnent"));
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
	int32 poolCount = width * height;
	TileComponent->TilePlanesComponent->PreWarmPool(poolCount);
	
}

void AGameBoard::InitializeBoard()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int32 index = GetIndexOfArray(x, y);
			SpawnPlane(x, y);
			UpdateTileInfoAndPlaneImage(index, GameplayTagEmptyTile);
		}
	}

	if (AGameStatePM* GameStatePM = Cast<AGameStatePM>(GetWorld()->GetGameState()))
	{
		GameStatePM->GameBoardSendBoardDimensionsDelegate.Broadcast(width, height);
		GameStatePM->GameBoardSendLevelPropertiesDelegate.Broadcast(ScoreComponent->totalAffectionNeeded, TileComponent->TilePopulatorComponent->GetLevelNumber());
		// GameStatePM->GameBoardPopulatedDelegate.Broadcast();
		// StartTimer();
		PopulateBoard();
		ProcessMatches();
	}
}

void AGameBoard::InitializeLevelProperties()
{
	if (ULevelData* LevelData = LevelDataManagerComponent->GetLevelData(LevelStage))
	{
		TileComponent->TilePopulatorComponent->SetLevel(LevelData->level);
		SetTimeNewRowAdd(LevelData->TimeToSpawnNewRow);
		ScoreComponent->SetTotalAffectionNeeded(LevelData->AffectionLevelMax);
	}
	else
	{
		UE_LOGFMT(LogTemp, Warning, "TilePopulatorComponent map is not set up correctly");
	}
	
}

void AGameBoard::OnLevelStarSetLevelStageAndtInitialze(ELevelStage InLevelStage)
{
	if (APlayerStatePM* PlayerStatePM = Cast<APlayerStatePM>(UGameplayStatics::GetPlayerState(GetWorld(),0)))
	{
		PlayerStatePM->ResetScoreToZero();
	}
	LevelStage = InLevelStage;
	InitializeLevelProperties();
	TileComponent->TilePlanesComponent->DestroyPlanes();
	InitializeBoard();
}

void AGameBoard::SpawnPlane(int x, int y)
{
	FVector PlaneSpawnLocation = GetTileLocationByXandY(x, y);
	TileComponent->TilePlanesComponent->SpawnPlaneAtLocation(PlaneSpawnLocation);
}

void AGameBoard::UpdateTileInfoAndPlaneImage(int32 index, FGameplayTag Gameplaytag)
{
	TileComponent->TileInfoManagerComponent->TileStatuses[index].TileInfo = TileComponent->TileInfoManagerComponent->GetTileInfo(Gameplaytag);
	TileComponent->TileInfoManagerComponent->ChangeTileStatus(index, TileComponent->TileInfoManagerComponent->TileStatuses[index]);
	TileComponent->TilePlanesComponent->ChangeTileImage(index, TileComponent->TileInfoManagerComponent->TileStatuses[index]);
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

void AGameBoard::OnLevelRestarted()
{
	if (APlayerStatePM* PlayerStatePM = Cast<APlayerStatePM>(UGameplayStatics::GetPlayerState(GetWorld(),0)))
	{
		PlayerStatePM->ResetScoreToZero();
	}
	ClearBoardData();
	PopulateBoard();
}

void AGameBoard::ClearBoardData()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int32 index = GetIndexOfArray(x, y);
			UpdateTileInfoAndPlaneImage(index, GameplayTagEmptyTile);
		}
	}
}

void AGameBoard::PopulateBoard()
{
	if (AGameStatePM* GameStatePM = Cast<AGameStatePM>(GetWorld()->GetGameState()))
	{
		GameStatePM->GameBoardPopulatedDelegate.Broadcast();
		StartTimer();
	}
}


// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AGameBoard::SwitchTiles(int32 indexLeft, int32 indexRight)
{

	// const FTransform TransformRight = TileComponent->TilePlanesComponent->BoardTiles[indexRight]->GetRelativeTransform();
	// const FTransform TransformLeft = TileComponent->TilePlanesComponent->BoardTiles[indexLeft]->GetRelativeTransform();
	// FLatentActionInfo LatentInfoRight;
	// LatentInfoRight.CallbackTarget = this;
	// LatentInfoRight.UUID = __LINE__;
	// LatentInfoRight.Linkage = 0;
	// LatentInfoRight.ExecutionFunction = NAME_None;
	//
	// UKismetSystemLibrary::MoveComponentTo(TileComponent->TilePlanesComponent->BoardTiles[indexLeft],TransformRight.GetLocation(), TransformRight.Rotator(),
	// 	false, false, 1.0f, false, EMoveComponentAction::Move, LatentInfoRight);
	//
	// FLatentActionInfo LatentInfoLeft;
	// LatentInfoLeft.CallbackTarget = this;
	// LatentInfoLeft.UUID = __LINE__;
	// LatentInfoLeft.Linkage = 0;
	// LatentInfoLeft.ExecutionFunction = NAME_None;
	//
	// UKismetSystemLibrary::MoveComponentTo(TileComponent->TilePlanesComponent->BoardTiles[indexRight],TransformLeft.GetLocation(), TransformLeft.Rotator(),
	// 	false, false, 1.0f, false, EMoveComponentAction::Move, LatentInfoLeft);

	FTileStatus RightStatus = TileComponent->TileInfoManagerComponent->TileStatuses[indexRight];
	FTileStatus LeftStatus = TileComponent->TileInfoManagerComponent->TileStatuses[indexLeft];

	TileComponent->TilePlanesComponent->SpawnPlaneAndSwitch(indexLeft, indexRight, RightStatus, false);

	
	TileComponent->TilePlanesComponent->SpawnPlaneAndSwitch(indexRight, indexLeft, LeftStatus, true);

	// FTileStatus LeftStatus = TileComponent->TileInfoManagerComponent->TileStatuses[indexLeft];
	// FTileStatus RightStatus = TileComponent->TileInfoManagerComponent->TileStatuses[indexRight];
	//
	// TileComponent->TileInfoManagerComponent->ChangeTileStatus(indexLeft, RightStatus);
	// TileComponent->TileInfoManagerComponent->ChangeTileStatus(indexRight, LeftStatus);
	//
	// TileComponent->TilePlanesComponent->ChangeTileImage(indexLeft, RightStatus);
	// TileComponent->TilePlanesComponent->ChangeTileImage(indexRight, LeftStatus);
	//
	// ProcessMatches();
	// TileComponent->TileInfoManagerComponent->CheckTilesBelowAndMove();
	// ProcessMatches();
}

void AGameBoard::DropPopulatedTilesAboveEmpty(int32 indexPopulated, int32 indexEmpty)
{
	FTileStatus EmptyStatus = TileComponent->TileInfoManagerComponent->TileStatuses[indexEmpty];
	FTileStatus PopulatedStatus = TileComponent->TileInfoManagerComponent->TileStatuses[indexPopulated];

	TileComponent->TilePlanesComponent->SpawnPlaneAndDrop(indexPopulated, indexEmpty, PopulatedStatus);
	//TileComponent->TilePlanesComponent->SpawnPlaneAndMove(indexEmpty, indexPopulated, PopulatedStatus);

	//Change populated tile to empty immediately
	TileComponent->TileInfoManagerComponent->ChangeTileStatus(indexPopulated, EmptyStatus);
	TileComponent->TilePlanesComponent->ChangeTileImage(indexPopulated, EmptyStatus);
	//change status, not image, of empty to populated
	TileComponent->TileInfoManagerComponent->ChangeTileStatus(indexEmpty, PopulatedStatus);
}

void AGameBoard::ProcessSwitch(int32 IndexCurrent, FTileStatus DestinationStatus, bool isSecondSwitch)
{
	FTileStatus CurrentStatus = TileComponent->TileInfoManagerComponent->TileStatuses[IndexCurrent];
	
	TileComponent->TileInfoManagerComponent->ChangeTileStatus(IndexCurrent, DestinationStatus);
	
	TileComponent->TilePlanesComponent->ChangeTileImage(IndexCurrent, DestinationStatus);
	
	if (isSecondSwitch == true)
	{
		TileComponent->TileInfoManagerComponent->CheckTilesBelowAndMove();
		ProcessMatches();
	}
	
}

void AGameBoard::ProcessDrop(int32 IndexDestination, FTileStatus CurrentStatus)
{
	
	TileComponent->TileInfoManagerComponent->ChangeTileStatus(IndexDestination, CurrentStatus);
	
	TileComponent->TilePlanesComponent->ChangeTileImage(IndexDestination, CurrentStatus);

	// ProcessMatches();
	// TileComponent->TileInfoManagerComponent->CheckTilesBelowAndMove();
	// ProcessMatches();
}

void AGameBoard::MoveTileRowsUpOneRow()
{
	FTileStatus EmptyStatus;
	EmptyStatus.TileInfo = TileComponent->TileInfoManagerComponent->TileInfoEmpty;

	TArray<FTileStatus> TileStatusesCopy = TileComponent->TileInfoManagerComponent->TileStatuses;
	int32 TotalTiles = TileComponent->TilePlanesComponent->BoardTiles.Num();

	GameOverCheck(TotalTiles, TileStatusesCopy);

	//move rows up. 
	for (int32 i = TotalTiles -1; i >= width; i--)
	{
		FTileStatus PopulatedStatus = TileComponent->TileInfoManagerComponent->TileStatuses[i - width];
		if (PopulatedStatus.TileInfo->GameplayTag == GameplayTagEmptyTile)
		{
			continue;
		}
		TileComponent->TilePlanesComponent->SpawnPlaneAndDrop(i-width, i, PopulatedStatus);
		TileComponent->TileInfoManagerComponent->ChangeTileStatus(i,TileStatusesCopy[i - width]);
		TileComponent->TilePlanesComponent->ChangeTileImage(i, EmptyStatus);
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

void AGameBoard::ProcessMatches()
{
	TileComponent->TileLineMatchProcessorComponent->CheckforLinesHorizontal();
	TileComponent->TileLineMatchProcessorComponent->CheckforLinesVertical();
	// CheckforLinesHorizontal();
	// CheckforLinesVertical();

	if (TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.IsEmpty() == false)
	{
		TileComponent->TileLineMatchProcessorComponent->ProcessMatches();
		TileComponent->TileInfoManagerComponent->CheckTilesBelowAndMove();
		ProcessMatches();
	}
}

void AGameBoard::GameOverCheck(int32 TotalTiles, TArray<FTileStatus> TileStatusesCopy)
{
	int32 LastRowStartingIndex = TotalTiles - width;

	for (int32 i = LastRowStartingIndex; i < TotalTiles; i++)
	{
		if (TileStatusesCopy[i].TileInfo && TileStatusesCopy[i].TileInfo->GameplayTag != GameplayTagEmptyTile)
		{
			UE_LOGFMT(LogTemp, Warning, "Game Over");
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

			if (APlayerStatePM* PlayerStatePM = Cast<APlayerStatePM>( UGameplayStatics::GetPlayerState(GetWorld(), 0)))
			{
				int32 livesRemaining = PlayerStatePM->GetPlayerLivesRemaining();
				if (livesRemaining <= 0)
				{
					if (AGameMode* GameMode = Cast<AGameMode>( UGameplayStatics::GetGameMode(GetWorld())))
					{
						GameMode->EndMatch();
					}
				}
				else
				{
					PlayerStatePM->ChangePlayerLivesRemaining(-1);
				}
			}
			return;
		}
	}
}

void AGameBoard::OnLevelCompletedStopBoard()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
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

void AGameBoard::SetTimeNewRowAdd(float InTimeNewRowAdd)
{
	timeNewRowAdd = InTimeNewRowAdd;
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
	ProcessMatches();
}



void AGameBoard::GetTileLocation(int32 tileIndex)
{
	FVector tileLocation = TileComponent->TilePlanesComponent->GetTileLocationByArrayIndex(tileIndex);
	DelegateBindingCompGameBoard->GameStatePM->GameBoardOnRequestSendTileLocationDelegate.Broadcast(tileLocation);
}






