// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Components/TileLineMatchProcessorComponent.h"
#include "Components/TileComponent.h"
#include "Data/TileStatus.h"
#include "GameBoard/GameBoard.h"

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
					}
				}
			}
		}
	}

	indexOfMatchedTiles.Empty();
}


// Called every frame
void UTileLineMatchProcessorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

