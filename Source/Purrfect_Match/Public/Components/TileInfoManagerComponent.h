// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/TileInfo.h"
#include "TileInfoManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURRFECT_MATCH_API UTileInfoManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileInfoManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info Empty")
	TObjectPtr<UTileInfo> TileInfoEmpty;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info Sun")
	TObjectPtr<UTileInfo> TileInfoAffectionSun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info Plany")
	TObjectPtr<UTileInfo> TileInfoAffectionPlant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info Toy")
	TObjectPtr<UTileInfo> TileInfoAffectionToy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info Food")
	TObjectPtr<UTileInfo> TileInfoAffectionFood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info Pet")
	TObjectPtr<UTileInfo> TileInfoGoalPet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info Bath")
	TObjectPtr<UTileInfo> TileInfoGoalBath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info Dog")
	TObjectPtr<UTileInfo> TileInfoGoalDog;

		
};
