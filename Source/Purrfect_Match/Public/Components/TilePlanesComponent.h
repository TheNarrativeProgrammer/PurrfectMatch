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

	virtual void InitializeComponent() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	TArray<UStaticMeshComponent*> BoardTiles;

	UFUNCTION(BlueprintCallable)
	void DestroyPlanes();

	UFUNCTION(BlueprintCallable)
	void SpawnPlaneAtLocation(FVector PlaneSpawnLocation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	UStaticMesh* PlaneMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	UMaterialInterface* BoardTileMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	UMaterialInterface* ScoreAffectionTileMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Board")
	UMaterialInterface* GoalNegativeTileMaterial;

	UFUNCTION(BlueprintCallable)
	FVector GetTileLocationByArrayIndex(int32 index);

	UFUNCTION()
	void ChangeTileImage(int32 IndexTile, FTileStatus NewStatus);

	UFUNCTION()
	void SpawnPlaneAndSwitch(int32 IndexCurrent, int32 IndexDestination, FTileStatus DestinationStatus, bool isSecondSwitch);

	UFUNCTION()
	void SpawnPlaneAndDrop(int32 IndexCurrent, int32 IndexDestination, FTileStatus CurrentPopulatedStatus);

	UFUNCTION()
	void SpawnScoreMaterialPlane(int32 IndexOfMatch, FGameplayTag GameplayTag);

	UFUNCTION()
	void SwitchPlanes(int32 IndexLeft, int32 IndexRight);

	UFUNCTION()
	void ToggleVisibilityOfTilePlane(int32 Index, bool IsVisible);

	UFUNCTION()
	void ChangeAppearanceOfPlaneToMimicEmpty(int32 Index);

	UFUNCTION(BlueprintCallable)
	void StopAllTimers();

	UFUNCTION(BlueprintCallable)
	void DestroyStaticMeshesPendingDestruction();

	UFUNCTION()
	void DestroyMovePlane(UStaticMeshComponent* StaticMeshComponent, float DestroyAfterDuration);

	UFUNCTION()
	void OnSwitchCompleteProcessSwitch(int32 IndexCurrent, FTileStatus DestinationStatus, float ProcessAfterDuration, bool isSecondSwitch);

	UFUNCTION()
	void OnDropCompleteProcessDrop(int32 IndexDestination, FTileStatus CurrentStatus, float ProcessAfterDuration);

	UFUNCTION()
	UStaticMeshComponent* SpawnMovementPlane(FTransform TransformForSpawn);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time of Plane Destruction")
	float movePlaneDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time of Plane Destruction")
	float ScorePlaneDuration = 0.4f;

	UPROPERTY()
	TArray<FTimerHandle> ActiveTimers;

	UPROPERTY()
	TArray<UStaticMeshComponent*> StaticMeshComponentsPendingDestuction;

	UFUNCTION()
	void AssignTileImageToMovePlane(int32 Index, UStaticMeshComponent* StaticMeshComponent);


	//pool
	UPROPERTY()
	TArray<UStaticMeshComponent*> StaticMeshComponentPoolAll;

	UPROPERTY()
	TArray<UStaticMeshComponent*> StaticMeshComponentPoolAvailable;

	UFUNCTION()
	void PreWarmPool(int32 PoolCount);

	UFUNCTION()
	UStaticMeshComponent* GetComponentFromPool(const FTransform& SpawnTransform);

	UFUNCTION()
	void ReturnComponentToPool(UStaticMeshComponent* Component);

	UFUNCTION()
	void EmptyPool();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
