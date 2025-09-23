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

