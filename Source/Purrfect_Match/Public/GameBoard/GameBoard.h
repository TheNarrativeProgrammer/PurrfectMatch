// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/TileInfo.h"
#include "GameFramework/Actor.h"
#include "GameplayTags.h"
#include "Components/TileInfoManagerComponent.h"
#include "GameBoard.generated.h"


USTRUCT()
struct FTileStatus
{
	GENERATED_BODY()
	UPROPERTY()
	bool bIsCleared = false;

	UPROPERTY()
	bool bIsOccupied = false;

	UPROPERTY()
	TObjectPtr<UTileInfo> TileInfo = nullptr;
};

USTRUCT(BlueprintType)
struct FMatchGroup
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<int32> indices;
};



class UTileInfo;
class UDelegateBindingCompGameBoard;

UCLASS()
class PURRFECT_MATCH_API AGameBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameBoard();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Delegate binding component")
	TObjectPtr<UDelegateBindingCompGameBoard> DelegateBindingCompGameBoard;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info")
	TObjectPtr<UTileInfoManagerComponent> TileInfoManagerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Points")
	int32 minimumMatchingForPoint = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Points")
	int32 pointsPerMatch = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Points")
	TArray<FMatchGroup> indexOfMatchedTiles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dimensions")
	int32 width = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dimensions")
	int32 height = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dimensions")
	int32 tileSize = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dimensions")
	int32 tileGap = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dimensions")
	int32 tileMoveAmount = tileSize + tileGap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dimensions")
	FGameplayTag GameplayTagEmptyTile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	TArray<UStaticMeshComponent*> BoardTiles;

	UPROPERTY(EditDefaultsOnly, Category = "Board")
	TArray<FTileStatus> TileStatuses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile info")
	UMaterialInterface* MaterialDefaultEmptyTile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	int32 indexOfArray = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	FVector GridOrigin = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	UMaterialInterface* BoardTileMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	UStaticMesh* PlaneMesh;

	UFUNCTION(BlueprintCallable)
	int32 GetIndexOfArray(int32 xValue, int32 yValue);

	UFUNCTION(BlueprintCallable)
	FVector GetTileLocationByXandY(int32 xValue, int32 yValue);

	UFUNCTION(BlueprintCallable)
	FVector GetTileLocationByArrayIndex(int32 index);

	UFUNCTION(BlueprintCallable)
	void SpawnPlaneAtLocation(FVector PlaneSpawnLocation);

	UFUNCTION(BlueprintCallable)
	void CheckforLinesHorizontal();

	UFUNCTION(BlueprintCallable)
	bool IsLineEmptyRow(int32 rowStartIndex);

	UFUNCTION(BlueprintCallable)
	void CheckforLinesVertical();

	UFUNCTION(BlueprintCallable)
	bool IsLineEmptyColumn(int32 columnStartIndex);

	UFUNCTION(BlueprintCallable)
	void ProcessMatches();

	

	

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	int32 GetWidth() const {return width;}

	UFUNCTION()
	void ChangeTileStatus(int32 IndexTile, FTileStatus NewStatus);

	UFUNCTION()
	void SwitchTiles(int32 indexLeft, int32 indexRight);

	UFUNCTION()
	void MoveTileRowsUpOneRow();

	UFUNCTION()
	void PopulateRow(int32 ColumnIndex, TArray<FGameplayTag> GameplayTags);

	UFUNCTION()
	UTileInfo* GetTileInfo(FGameplayTag GameplayTag);

	UFUNCTION()
	void GetTileLocation(int32 tileIndex);

};
