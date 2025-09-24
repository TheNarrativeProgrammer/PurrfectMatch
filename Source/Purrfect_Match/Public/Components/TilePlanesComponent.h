// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/TileStatus.h"
#include "TilePlanesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURRFECT_MATCH_API UTilePlanesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTilePlanesComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	TArray<UStaticMeshComponent*> BoardTiles;

	UFUNCTION(BlueprintCallable)
	void SpawnPlaneAtLocation(FVector PlaneSpawnLocation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	UStaticMesh* PlaneMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	UMaterialInterface* BoardTileMaterial;

	UFUNCTION(BlueprintCallable)
	FVector GetTileLocationByArrayIndex(int32 index);

	UFUNCTION()
	void ChangeTileImage(int32 IndexTile, FTileStatus NewStatus);

	UFUNCTION()
	void MovePlaneDown(int32 IndexCurrent, int32 IndexDestination);

	UFUNCTION()
	void SwitchPlanes(int32 IndexLeft, int32 IndexRight);

	UFUNCTION()
	void ToggleVisibilityOfTilePlane(int32 Index, bool IsVisible);

	UFUNCTION()
	void ChangeAppearanceOfPlaneToMimicEmpty(int32 Index);

	UFUNCTION()
	void DestroyMovePlane(UStaticMeshComponent* StaticMeshComponent, float DestroyAfterDuration);

	UFUNCTION()
	UStaticMeshComponent* SpawnMovementPlane(FTransform TransformForSpawn);

	// UPROPERTY()
	// TObjectPtr<UStaticMeshComponent> MoveStaticMeshComponent;
	

	UPROPERTY()
	float movePlaneDuration = 2.0f;

	UPROPERTY()
	TArray<FTimerHandle> ActiveTimers;

	UFUNCTION()
	void AssignTileImageToMovePlane(int32 Index, UStaticMeshComponent* StaticMeshComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
