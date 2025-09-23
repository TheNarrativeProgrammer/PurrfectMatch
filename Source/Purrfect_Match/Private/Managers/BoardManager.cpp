// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/BoardManager.h"
#include "GameBoard/GameBoard.h"
#include "components/Binding/DelegateBindingCompBoardManager.h"

// Sets default values
ABoardManager::ABoardManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DelegateBindingCompBoardManager = CreateDefaultSubobject<UDelegateBindingCompBoardManager>(TEXT("DelegateBindingCompBoardManager"));

	TagAffectionFood = FGameplayTag::RequestGameplayTag(FName("Affection.Food"));
	TagAffectionPlant = FGameplayTag::RequestGameplayTag(FName("Affection.Plants"));
	TagAffectionSun = FGameplayTag::RequestGameplayTag(FName("Affection.Sun"));
	TagAffectionToy = FGameplayTag::RequestGameplayTag(FName("Affection.Toy"));
	TagGoalBath = FGameplayTag::RequestGameplayTag(FName("Goal.Bath"));
	TagGoalDog = FGameplayTag::RequestGameplayTag(FName("Goal.Dog"));
	TagGoalPet = FGameplayTag::RequestGameplayTag(FName("Goal.Pet"));

}

// Called when the game starts or when spawned
void ABoardManager::BeginPlay()
{
	Super::BeginPlay();

	
}

TArray<FGameplayTag> ABoardManager::GenerateTileLine()
{
	TArray<FGameplayTag> NewRowTags;
	for (int32 index = 0; index < GameBoard->GetWidth(); index++)
	{
		if (FMath::FRand() < chanceOfGoalTile)
		{
			NewRowTags.Add(GetTileTag(index));
		}
		else
		{
			int32 RandomNumber = FMath::RandRange(0, 3);
			NewRowTags.Add(GetTileTag(RandomNumber));
		}
	}
	return NewRowTags;
}

FGameplayTag ABoardManager::GetTileTag(int32 tileIconNumber)
{
	switch (tileIconNumber)
	{
		case 0:
			{
				return TagAffectionSun;
			}
		case 1:
			{
				return TagAffectionFood;
			}
		case 2:
			{
				return TagAffectionPlant;
			}
		case 3:
			{
				return TagAffectionToy;
			}
		case 4:
			{
				if (level == 1)
				{
					return TagGoalBath;
				}
				else if (level == 2)
				{
					return TagGoalPet;
				}
				else if (level == 3)
				{
					return TagGoalDog;
				}
			}
		default:
			{
				return TagAffectionFood;
			}
	}
}

// Called every frame
void ABoardManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardManager::PopulateBoardWithInitialTiles()
{
	for (int i = 0; i < numberOfRowsAtLevelStart; i++)
	{
		TArray<FGameplayTag> NewTileLine = GenerateTileLine();
		GameBoard->PopulateRow(i, NewTileLine);
	}
}

