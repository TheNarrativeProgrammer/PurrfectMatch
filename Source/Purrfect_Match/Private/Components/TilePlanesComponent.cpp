// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TilePlanesComponent.h"

// Sets default values for this component's properties
UTilePlanesComponent::UTilePlanesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTilePlanesComponent::SpawnPlaneAtLocation(FVector PlaneSpawnLocation)
{
	FTransform PlaneSpawnTransform;
	PlaneSpawnTransform.SetLocation(PlaneSpawnLocation);

	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		UActorComponent* ActorComponent = ActorOwner->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, PlaneSpawnTransform, false);

		if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponent))
		{
			FRotator Rotator = FRotator(0.0f, 00.0f, -90.0f);
			StaticMeshComponent->SetWorldRotation(Rotator);
			if (PlaneMesh)
			{
				StaticMeshComponent->SetStaticMesh(PlaneMesh);
			}
			StaticMeshComponent->RegisterComponent();
			if (BoardTileMaterial)
			{
				StaticMeshComponent->SetMaterial(0, BoardTileMaterial);
			}
			BoardTiles.AddUnique(StaticMeshComponent);
			StaticMeshComponent->SetupAttachment( ActorOwner->GetRootComponent());
		}
	}
	
}

FVector UTilePlanesComponent::GetTileLocationByArrayIndex(int32 index)
{
	if (BoardTiles.IsValidIndex(index))
	{
		return BoardTiles[index]->GetComponentLocation();
	}
	return BoardTiles[0]->GetComponentLocation();
}

void UTilePlanesComponent::ChangeTileImage(int32 IndexTile, FTileStatus NewStatus)
{
	BoardTiles[IndexTile]->SetMaterial(0, NewStatus.TileInfo->Material);
}

// Called when the game starts
void UTilePlanesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTilePlanesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

