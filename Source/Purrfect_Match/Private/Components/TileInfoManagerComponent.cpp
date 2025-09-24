// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/TileInfoManagerComponent.h"

#include "Components/TilePlanesComponent.h"
#include "GameBoard/GameBoard.h"
#include "Logging/StructuredLog.h"


// Sets default values for this component's properties
UTileInfoManagerComponent::UTileInfoManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	GameplayTagEmptyTile = FGameplayTag::RequestGameplayTag(FName("EmptyTile"));

	// ...
}

void UTileInfoManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (AGameBoard* GameBoard = Cast<AGameBoard>(GetOwner()))
	{
		if (GameBoard->Implements<UBoardable>())
		{
			TileStatuses.SetNum(IBoardable::Execute_GetBoardWidth(GameBoard) * GameBoard->Execute_GetBoardHeight(GameBoard));
		}
		
	}
	
}


// Called when the game starts
void UTileInfoManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTileInfoManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UTileInfo* UTileInfoManagerComponent::GetTileInfo(FGameplayTag GameplayTag)
{
	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("EmptyTile")))
	{
		return TileInfoEmpty;
	}
	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Sun")))
	{
		return TileInfoAffectionSun;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Toy")))
	{
		return TileInfoAffectionToy;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Plants")))
	{
		return TileInfoAffectionPlant;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Affection.Food")))
	{
		return TileInfoAffectionFood;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Bath")))
	{
		return TileInfoGoalBath;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Pet")))
	{
		return TileInfoGoalPet;
	}

	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("Goal.Dog")))
	{
		return TileInfoGoalDog;
	}
	return TileInfoAffectionSun;
}

void UTileInfoManagerComponent::ChangeTileStatus(int32 IndexTile, FTileStatus NewStatus)
{
	if (NewStatus.TileInfo == nullptr)
	{
		UE_LOGFMT(LogTemp, Warning, "Missing empty DA in the TileInfo Component. TileInfo withing TileStatus is nullptr for empty tiles");
		return;
	}
	TileStatuses[IndexTile] = NewStatus;
}

bool UTileInfoManagerComponent::IsTileBelowIsEmpty(int32 IndexTile, int32& LowestEmptyTile)
{
	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		if (ActorOwner->Implements<UBoardable>())
		{
			int32 width = IBoardable::Execute_GetBoardWidth(ActorOwner);
			
					int32 CurrentIndex = IndexTile - width;
					int32 LastValidEmptyIndex = -1;
					
					while (CurrentIndex >= 0 && TileStatuses.IsValidIndex(CurrentIndex))
					{
						if (TileStatuses[CurrentIndex].TileInfo && 
							TileStatuses[CurrentIndex].TileInfo->GameplayTag == GameplayTagEmptyTile)
						{
							LastValidEmptyIndex = CurrentIndex;
							CurrentIndex -= width;
						}
						else
						{
							break;
						}
					}

					if (LastValidEmptyIndex != -1)
					{
						LowestEmptyTile = LastValidEmptyIndex;
						return true;
					}
				}
			}
		
	
	
			return false;
	
}

void UTileInfoManagerComponent::CheckTilesBelowAndMove()
{
	for (int32 Index = 6; Index < TileStatuses.Num(); Index++)
	{
		if (TileStatuses[Index].TileInfo->GameplayTag == GameplayTagEmptyTile)
		{
			continue;
		}

		int32 LowestEmptyTile = 0;
		if (IsTileBelowIsEmpty(Index, LowestEmptyTile))
		{
			if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
			{
				if (UTilePlanesComponent* TilePlanesComponent = ActorOwner->GetComponentByClass<UTilePlanesComponent>())
				{
					//TilePlanesComponent->MovePlaneDown(Index, LowestEmptyTile);
				}
			}
			MoveTileDown(Index, LowestEmptyTile);
		}
	}
}

void UTileInfoManagerComponent::MoveTileDown(int32 currentIndex, int32 NewIndex)
{
	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		if (UTileComponent* TileComponent = ActorOwner->GetComponentByClass<UTileComponent>())
		{
			FTileStatus NonEmptyStatus = TileComponent->TileInfoManagerComponent->TileStatuses[currentIndex];
			FTileStatus EmptyStatus = TileComponent->TileInfoManagerComponent->TileStatuses[NewIndex];
	
			TileComponent->TileInfoManagerComponent->ChangeTileStatus(currentIndex, EmptyStatus);
			TileComponent->TileInfoManagerComponent->ChangeTileStatus(NewIndex, NonEmptyStatus);

			TileComponent->TilePlanesComponent->ChangeTileImage(currentIndex, EmptyStatus);
			TileComponent->TilePlanesComponent->ChangeTileImage(NewIndex, NonEmptyStatus);

			// TileComponent->TileLineMatchProcessorComponent->CheckforLinesHorizontal();
			// TileComponent->TileLineMatchProcessorComponent->CheckforLinesVertical();

			//
			// if (TileComponent->TileLineMatchProcessorComponent->indexOfMatchedTiles.IsEmpty() == false)
			// {
			// 	TileComponent->TileLineMatchProcessorComponent->ProcessMatches();
			
		}
	}
	
	
}

