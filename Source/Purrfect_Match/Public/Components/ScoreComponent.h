// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURRFECT_MATCH_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScoreComponent();
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time")
	int32 totalAffectionNeeded  = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Points")
	int32 minimumMatchingForPoint = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Points")
	int32 pointsPerMatch = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tracking bool")
	bool isChanceOfGoalTileUpdated = false;

	UFUNCTION(BlueprintCallable)
	void UpdateScore(int32 pointsScored, FGameplayTag GameplayTag);

	UFUNCTION()
	void SetTotalAffectionNeeded(int32 InTotalAffectionNeeded);

	UFUNCTION()
	void SetIsChanceOfGoalTileUpdated(bool InIsChangeOfGoalTileUpdated);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
