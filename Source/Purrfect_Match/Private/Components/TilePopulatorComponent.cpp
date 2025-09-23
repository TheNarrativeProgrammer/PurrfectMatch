// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TilePopulatorComponent.h"
#include "GameBoard/GameBoard.h"
#include "Components/Binding/DelegateBindingCompBoardManager.h"

// Sets default values for this component's properties
UTilePopulatorComponent::UTilePopulatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	DelegateBindingCompBoardManager = CreateDefaultSubobject<UDelegateBindingCompBoardManager>(TEXT("DelegateBindingCompBoardManager"));

	
}


// Called when the game starts
void UTilePopulatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTilePopulatorComponent::InitializeComponent()
{
	Super::InitializeComponent();
	TagAffectionFood = FGameplayTag::RequestGameplayTag(FName("Affection.Food"));
	TagAffectionPlant = FGameplayTag::RequestGameplayTag(FName("Affection.Plants"));
	TagAffectionSun = FGameplayTag::RequestGameplayTag(FName("Affection.Sun"));
	TagAffectionToy = FGameplayTag::RequestGameplayTag(FName("Affection.Toy"));
	TagGoalBath = FGameplayTag::RequestGameplayTag(FName("Goal.Bath"));
	TagGoalDog = FGameplayTag::RequestGameplayTag(FName("Goal.Dog"));
	TagGoalPet = FGameplayTag::RequestGameplayTag(FName("Goal.Pet"));

	if (AGameBoard* GameBoardOwner = Cast<AGameBoard>( GetOwner()))
	{
		GameBoard = GameBoardOwner;
	}
}


TArray<FGameplayTag> UTilePopulatorComponent::GenerateTileLine()
{
	TArray<FGameplayTag> NewRowTags;
	if (GameBoard->Implements<UBoardable>())
	{
		for (int32 index = 0; index < IBoardable::Execute_GetBoardWidth(GameBoard); index++)
		{
			if (FMath::FRand() < chanceOfGoalTile)
			{
				NewRowTags.Add(GetTileTag(4));
			}
			else
			{
				int32 RandomNumber = FMath::RandRange(0, 3);
				NewRowTags.Add(GetTileTag(RandomNumber));
			}
		}
	}
	
	return NewRowTags;
}

FGameplayTag UTilePopulatorComponent::GenerateTileSingle()
{
	if (FMath::FRand() < chanceOfGoalTile)
	{
		return GetTileTag(4);
	}
	else
	{
		int32 RandomNumber = FMath::RandRange(0, 3);
		return GetTileTag(RandomNumber);
	}
}


FGameplayTag UTilePopulatorComponent::GetTileTag(int32 tileIconNumber)
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

void UTilePopulatorComponent::PopulateBoardWithInitialTiles()
{
	for (int i = 0; i < numberOfRowsAtLevelStart; i++)
	{
		TArray<FGameplayTag> NewTileLine = GenerateTileLine();
		GameBoard->PopulateRow(i, NewTileLine);
	}
}

// Called every frame
void UTilePopulatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

