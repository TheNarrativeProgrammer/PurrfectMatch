// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "TileLineMatchProcessorComponent.generated.h"

USTRUCT(BlueprintType)
struct FMatchGroup
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<int32> indices;
};

class UTileComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURRFECT_MATCH_API UTileLineMatchProcessorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileLineMatchProcessorComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Points")
	TArray<FMatchGroup> indexOfMatchedTiles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dimensions")
	FGameplayTag GameplayTagEmptyTile;

	UFUNCTION(BlueprintCallable)
	void ProcessMatches();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
