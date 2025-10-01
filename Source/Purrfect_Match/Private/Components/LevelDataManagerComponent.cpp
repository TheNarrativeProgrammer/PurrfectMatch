// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LevelDataManagerComponent.h"

// Sets default values for this component's properties
ULevelDataManagerComponent::ULevelDataManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevelDataManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULevelDataManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ULevelData* ULevelDataManagerComponent::GetLevelData(TEnumAsByte<ELevelStage> LevelState)
{
	if (LevelMap.Contains(LevelState))
	{
		ULevelData* Data = LevelMap[LevelState];
		return Data;
	}
	return nullptr;
}

