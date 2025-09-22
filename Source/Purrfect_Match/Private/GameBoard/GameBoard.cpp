// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard/GameBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Core/GameStatePM.h"
#include "Components/Binding/DelegateBindingCompGameBoard.h"
#include "Logging/StructuredLog.h"
#include "SparseVolumeTexture/SparseVolumeTexture.h"

// Sets default values
AGameBoard::AGameBoard()
{
	TileInfoManagerComponent = CreateDefaultSubobject<UTileInfoManagerComponent>(TEXT("Tile Info"));

	DelegateBindingCompGameBoard = CreateDefaultSubobject<UDelegateBindingCompGameBoard>(TEXT("DelegateBindingCompGameBoard"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileStatuses.SetNum(width * height);

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
			SpawnPlaneAtLocation(PlaneSpawnLocation);
			FGameplayTag GameBoardGameTag = FGameplayTag::RequestGameplayTag(FName("EmptyTile"));
			TileStatuses[index].TileInfo = GetTileInfo(GameBoardGameTag);
			ChangeTileStatus(index, TileStatuses[index]);
		}
	}

	if (AGameStatePM* GameStatePM = Cast<AGameStatePM>(GetWorld()->GetGameState()))
	{
		GameStatePM->GameBoardSendBoardDimensionsDelegate.Broadcast(width, height);
		GameStatePM->GameBoardPopulatedDelegate.Broadcast();
		
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

FVector AGameBoard::GetTileLocationByArrayIndex(int32 index)
{
	if (BoardTiles.IsValidIndex(index))
	{
		return BoardTiles[index]->GetComponentLocation();
	}
	return BoardTiles[0]->GetComponentLocation();
}

void AGameBoard::SpawnPlaneAtLocation(FVector PlaneSpawnLocation)
{
	FTransform PlaneSpawnTransform;
	PlaneSpawnTransform.SetLocation(PlaneSpawnLocation);
	
	UActorComponent* ActorComponent = AddComponentByClass(UStaticMeshComponent::StaticClass(), false, PlaneSpawnTransform, false);

	if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponent))
	{
		FRotator Rotator = FRotator(0.0f, 00.0f, -90.0f);
		StaticMeshComponent->SetWorldRotation(Rotator);
		if (PlaneMesh)
		{
			StaticMeshComponent->SetStaticMesh(PlaneMesh);
		}
		StaticMeshComponent->RegisterComponent();
		if (BoardTileMaterial)
		{
			StaticMeshComponent->SetMaterial(0, BoardTileMaterial);
		}
		BoardTiles.AddUnique(StaticMeshComponent);
		
	}
}

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameBoard::ChangeTileStatus(int32 IndexTile, FTileStatus NewStatus)
{
	if (NewStatus.TileInfo == nullptr)
	{
		UE_LOGFMT(LogTemp, Warning, "Missing empty DA in the TileInfo Component. TileInfo withing TileStatus is nullptr for empty tiles");
		return;
	}
	TileStatuses[IndexTile] = NewStatus;
	BoardTiles[IndexTile]->SetMaterial(0, NewStatus.TileInfo->Material);
}

void AGameBoard::SwitchTiles(int32 indexLeft, int32 indexRight)
{
	FTileStatus LeftStatus = TileStatuses[indexLeft];
	FTileStatus RightStatus = TileStatuses[indexRight];

	ChangeTileStatus(indexLeft, RightStatus);
	ChangeTileStatus(indexRight, LeftStatus);
}

void AGameBoard::MoveTileRowsUpOneRow()
{
	for (int32 index = 0; index < BoardTiles.Num(); index++)
	{
		int32 newIndex = index + width;
		ChangeTileStatus(index,TileStatuses[index]);
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
		NewStatus.TileInfo = GetTileInfo(GameplayTags[tagindex]);
		NewStatus.bIsCleared = false;
		NewStatus.bIsOccupied = true;
		ChangeTileStatus(index, NewStatus);
		tagindex++;
	}
}

UTileInfo* AGameBoard::GetTileInfo(FGameplayTag GameplayTag)
{
	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("EmptyTile")))
	{
		return TileInfoManagerComponent->TileInfoEmpty;
	}
	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Sun")))
	{
		return TileInfoManagerComponent->TileInfoAffectionSun;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Toy")))
	{
		return TileInfoManagerComponent->TileInfoAffectionToy;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Plants")))
	{
		return TileInfoManagerComponent->TileInfoAffectionPlant;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Food")))
	{
		return TileInfoManagerComponent->TileInfoAffectionFood;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Bath")))
	{
		return TileInfoManagerComponent->TileInfoGoalBath;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Pet")))
	{
		return TileInfoManagerComponent->TileInfoGoalPet;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Dog")))
	{
		return TileInfoManagerComponent->TileInfoGoalDog;
	}
	return TileInfoManagerComponent->TileInfoAffectionSun;

	
}

void AGameBoard::GetTileLocation(int32 tileIndex)
{
	FVector tileLocation = GetTileLocationByArrayIndex(tileIndex);
	DelegateBindingCompGameBoard->GameStatePM->GameBoardOnRequestSendTileLocationDelegate.Broadcast(tileLocation);
}






