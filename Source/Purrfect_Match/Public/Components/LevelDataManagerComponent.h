// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/GameInstancePM.h"
#include "Data/LevelData.h"
#include "LevelDataManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURRFECT_MATCH_API ULevelDataManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelDataManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Blueprintable, Category = "Level Data")
	TArray<ULevelData*> LevelDatas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Data")
	TMap<TEnumAsByte<ELevelStage>, ULevelData*> LevelMap;

	UFUNCTION()
	ULevelData* GetLevelData(TEnumAsByte<ELevelStage> LevelState);
};
