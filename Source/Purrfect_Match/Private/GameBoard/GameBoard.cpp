// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard/GameBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Tests/AutomationEditorCommon.h"

// Sets default values
AGameBoard::AGameBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameBoard::BeginPlay()
{
	Super::BeginPlay();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int32 index = GetIndexOfArray(x, y);
			FVector PlaneSpawnLocation = GetPlaneLocation(x, y);
			SpawnPlaneAtLocation(PlaneSpawnLocation);
		}
	}
	
}

int32 AGameBoard::GetIndexOfArray(int32 xValue, int32 yValue)
{
	return yValue * width + xValue;
}

FVector AGameBoard::GetPlaneLocation(int32 xValue, int32 yValue)
{
	float x = (GridOrigin.X + xValue) * tileMoveAmount;
	float z = (GridOrigin.Z + yValue) * tileMoveAmount;

	return FVector(x, 0.0f, z);
}

void AGameBoard::SpawnPlaneAtLocation(FVector PlaneSpawnLocation)
{
	FTransform PlaneSpawnTransform;
	PlaneSpawnTransform.SetLocation(PlaneSpawnLocation);
	
	UActorComponent* ActorComponent = AddComponentByClass(UStaticMeshComponent::StaticClass(), false, PlaneSpawnTransform, false);

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
	}
}

// Called every frame
void AGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

