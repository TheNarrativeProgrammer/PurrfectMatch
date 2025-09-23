// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TileComponent.h"
#include "Components/TilePopulatorComponent.h"
#include "Components/TileInfoManagerComponent.h"
#include "Components/TilePlanesComponent.h"
#include "Components/TileLineMatchProcessorComponent.h"

// Sets default values for this component's properties
UTileComponent::UTileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TileInfoManagerComponent = CreateDefaultSubobject<UTileInfoManagerComponent>(TEXT("Tile Info"));
	TilePopulatorComponent = CreateDefaultSubobject<UTilePopulatorComponent>(TEXT("TilePopulatorComponent"));
	
	TileLineMatchProcessorComponent = CreateDefaultSubobject<UTileLineMatchProcessorComponent>(TEXT("TileLineMatchProcessor"));
	TilePlanesComponent = CreateDefaultSubobject<UTilePlanesComponent>(TEXT("TilePlanesComponent"));

	// ...
}


// Called when the game starts
void UTileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

