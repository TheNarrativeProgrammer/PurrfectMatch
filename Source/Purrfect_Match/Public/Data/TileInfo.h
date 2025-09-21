// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TileInfo.generated.h"


UCLASS()
class PURRFECT_MATCH_API UTileInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	UTexture2D* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	UMaterialInterface* Material;
};
