// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "Data/TileInfo.h"
#include "TileStatus.generated.h"
/**
 * 
 */
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

class PURRFECT_MATCH_API TileStatus
{
public:
	TileStatus();
	~TileStatus();
};
