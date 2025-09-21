// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/PawnPlayerPM.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APawnPlayerPM::APawnPlayerPM()
{
	SceneComponentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentRoot"));
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	
	SetRootComponent(SceneComponentRoot);
	StaticMesh->SetupAttachment(SceneComponentRoot);

	SceneComponentRoot->SetMobility(EComponentMobility::Type::Movable);
	StaticMesh->SetMobility(EComponentMobility::Type::Movable);
	
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APawnPlayerPM::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnPlayerPM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnPlayerPM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

