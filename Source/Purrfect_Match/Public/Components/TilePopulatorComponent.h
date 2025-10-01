// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include  "GameplayTags.h"
#include "TilePopulatorComponent.generated.h"


class AGameBoard;
class UDelegateBindingCompBoardManager;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURRFECT_MATCH_API UTilePopulatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTilePopulatorComponent();

	UFUNCTION(BlueprintCallable)
	int32 GetLevelNumber() const {return level;}

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding Component")
	TObjectPtr<UDelegateBindingCompBoardManager> DelegateBindingCompBoardManager;

	UPROPERTY(EditAnywhere)
	int32 level = 1;

	UPROPERTY()
	FGameplayTag TagAffectionSun;

	UPROPERTY()
	FGameplayTag TagAffectionPlant;

	UPROPERTY()
	FGameplayTag TagAffectionFood;

	UPROPERTY()
	FGameplayTag TagAffectionToy;

	UPROPERTY()
	FGameplayTag TagGoalBath;

	UPROPERTY()
	FGameplayTag TagGoalPet;

	UPROPERTY()
	FGameplayTag TagGoalDog;

	UPROPERTY()
	float chanceOfGoalTile = 0.2f;

	UPROPERTY()
	int32 numberOfRowsAtLevelStart = 3;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Board Reference")
	TObjectPtr<AGameBoard> GameBoard = nullptr;

	

	UFUNCTION(BlueprintCallable)
	FGameplayTag GenerateTileSingle();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetTileTag(int32 tileIconNumber);

public:

	UFUNCTION(BlueprintCallable)
	void SetLevel(int32 InLevel);

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayTag> GenerateTileLine();

	UFUNCTION()
	void PopulateBoardWithInitialTiles();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
