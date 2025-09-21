// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameBoard.generated.h"


USTRUCT()
struct FTileInfo
{
	GENERATED_BODY()
	UPROPERTY()
	bool bIsCleared = false;
	
};
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	TArray<UStaticMeshComponent*> BoardTiles;

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
	FVector GetPlaneLocation(int32 xValue, int32 yValue);

	UFUNCTION(BlueprintCallable)
	void SpawnPlaneAtLocation(FVector PlaneSpawnLocation);

	

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
