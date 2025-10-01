// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TilePlanesComponent.h"

#include "NaniteSceneProxy.h"
#include "Components/TileInfoManagerComponent.h"
#include "GameBoard/GameBoard.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UTilePlanesComponent::UTilePlanesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTilePlanesComponent::DestroyPlanes()
{
	if (BoardTiles.IsEmpty())
	{
		return;
	}
	for (int32 i = 0; i < BoardTiles.Num(); i++)
	{
		BoardTiles[i]->DestroyComponent();
	}
	BoardTiles.Empty();
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

void UTilePlanesComponent::SpawnPlaneAndSwitch(int32 IndexCurrent, int32 IndexDestination, FTileStatus DestinationStatus, bool isSecondSwitch)
{
	if (BoardTiles.IsValidIndex(IndexCurrent) == false && BoardTiles.IsValidIndex(IndexDestination) == false)
	{
		return;
	}
	FTransform PlaneSpawnTransform = BoardTiles[IndexCurrent]->GetRelativeTransform();
	
	UStaticMeshComponent* MoveStaticMeshComponent = SpawnMovementPlane(PlaneSpawnTransform);
	if (!MoveStaticMeshComponent) return;
	
	AssignTileImageToMovePlane(IndexCurrent, MoveStaticMeshComponent);
	ChangeAppearanceOfPlaneToMimicEmpty(IndexCurrent);

	const FTransform TransformDesination = BoardTiles[IndexDestination]->GetRelativeTransform();

	static int32 MoveUUID = 1000;
	FLatentActionInfo LatentInfoDestination;
	LatentInfoDestination.CallbackTarget = this;
	LatentInfoDestination.UUID = MoveUUID++;
	LatentInfoDestination.Linkage = 0;
	LatentInfoDestination.ExecutionFunction = NAME_None;

	UKismetSystemLibrary::MoveComponentTo(MoveStaticMeshComponent,TransformDesination.GetLocation(), TransformDesination.Rotator(),
		false, false, movePlaneDuration, false, EMoveComponentAction::Move, LatentInfoDestination);

	DestroyMovePlane(MoveStaticMeshComponent, movePlaneDuration + 0.05f);

	if (isSecondSwitch == true)
	{
		OnSwitchCompleteProcessSwitch(IndexCurrent, DestinationStatus, movePlaneDuration + 0.07f, isSecondSwitch);
	}
	else
	{

		OnSwitchCompleteProcessSwitch(IndexCurrent, DestinationStatus, movePlaneDuration + 0.05f, isSecondSwitch);
		
	}
	
	
	
}


void UTilePlanesComponent::SpawnPlaneAndDrop(int32 IndexCurrent, int32 IndexDestination, FTileStatus CurrentPopulatedStatus)
{
	if (BoardTiles.IsValidIndex(IndexCurrent) == false && BoardTiles.IsValidIndex(IndexDestination) == false)
	{
		return;
	}
	FTransform PlaneSpawnTransform = BoardTiles[IndexCurrent]->GetRelativeTransform();
	
	UStaticMeshComponent* MoveStaticMeshComponent = SpawnMovementPlane(PlaneSpawnTransform);
	if (!MoveStaticMeshComponent) return;
	
	AssignTileImageToMovePlane(IndexCurrent, MoveStaticMeshComponent);
	//ChangeAppearanceOfPlaneToMimicEmpty(IndexCurrent);

	const FTransform TransformDesination = BoardTiles[IndexDestination]->GetRelativeTransform();

	static int32 MoveUUID = 1000;
	FLatentActionInfo LatentInfoDestination;
	LatentInfoDestination.CallbackTarget = this;
	LatentInfoDestination.UUID = MoveUUID++;
	LatentInfoDestination.Linkage = 0;
	LatentInfoDestination.ExecutionFunction = NAME_None;

	UKismetSystemLibrary::MoveComponentTo(MoveStaticMeshComponent,TransformDesination.GetLocation(), TransformDesination.Rotator(),
		false, false, movePlaneDuration, false, EMoveComponentAction::Move, LatentInfoDestination);

	DestroyMovePlane(MoveStaticMeshComponent, movePlaneDuration + 0.1f);
	OnDropCompleteProcessDrop(IndexDestination, CurrentPopulatedStatus, movePlaneDuration + 0.1f);
}

void UTilePlanesComponent::SwitchPlanes(int32 indexLeft, int32 indexRight)
{
	const FTransform TransformRight = BoardTiles[indexRight]->GetRelativeTransform();
	const FTransform TransformLeft = BoardTiles[indexLeft]->GetRelativeTransform();
	FLatentActionInfo LatentInfoRight;
	LatentInfoRight.CallbackTarget = this;
	LatentInfoRight.UUID = __LINE__;
	LatentInfoRight.Linkage = 0;
	LatentInfoRight.ExecutionFunction = NAME_None;
	
	UKismetSystemLibrary::MoveComponentTo(BoardTiles[indexLeft],TransformRight.GetLocation(), TransformRight.Rotator(),
		false, false, 1.0f, false, EMoveComponentAction::Move, LatentInfoRight);
	
	FLatentActionInfo LatentInfoLeft;
	LatentInfoLeft.CallbackTarget = this;
	LatentInfoLeft.UUID = __LINE__;
	LatentInfoLeft.Linkage = 0;
	LatentInfoLeft.ExecutionFunction = NAME_None;
	
	UKismetSystemLibrary::MoveComponentTo(BoardTiles[indexRight],TransformLeft.GetLocation(), TransformLeft.Rotator(),
		false, false, 1.0f, false, EMoveComponentAction::Move, LatentInfoLeft);
}

void UTilePlanesComponent::ToggleVisibilityOfTilePlane(int32 Index, bool IsVisible)
{
	BoardTiles[Index]->SetVisibility(false);
	
}

void UTilePlanesComponent::ChangeAppearanceOfPlaneToMimicEmpty(int32 Index)
{
	if (BoardTileMaterial)
	{
		BoardTiles[Index]->SetMaterial(0, BoardTileMaterial);
	}
}

void UTilePlanesComponent::DestroyMovePlane(UStaticMeshComponent* StaticMeshComponent, float DestroyAfterDuration)
{
	if (!StaticMeshComponent) return;
	
	FTimerHandle TimerHandle;
    
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this, StaticMeshComponent, TimerHandle]()
		{
			if (StaticMeshComponent)
			{
				StaticMeshComponent->DestroyComponent();
				ActiveTimers.Remove(TimerHandle);
			}
		},
		DestroyAfterDuration,  // Delay per component
		false  // Do not loop
	);
	
	ActiveTimers.Add(TimerHandle);
}

void UTilePlanesComponent::OnSwitchCompleteProcessSwitch(int32 IndexCurrent, FTileStatus DestinationStatus, float ProcessAfterDuration, bool isSecondSwitch)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this, IndexCurrent, DestinationStatus, TimerHandle, isSecondSwitch]()
		{
			if (AActor* ActorOwner = GetOwner())
			{
				if (AGameBoard* GameBoard = Cast<AGameBoard>(ActorOwner))
				{
					GameBoard->ProcessSwitch(IndexCurrent, DestinationStatus, isSecondSwitch);
					ActiveTimers.Remove(TimerHandle);
				}
			}
		},
			ProcessAfterDuration, false
	);
	ActiveTimers.Add(TimerHandle);
}

void UTilePlanesComponent::OnDropCompleteProcessDrop(int32 IndexDestination, FTileStatus CurrentStatus,
	float ProcessAfterDuration)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this, IndexDestination, CurrentStatus, TimerHandle]()
		{
			if (AActor* ActorOwner = GetOwner())
			{
				if (AGameBoard* GameBoard = Cast<AGameBoard>(ActorOwner))
				{
					GameBoard->ProcessDrop(IndexDestination, CurrentStatus);
					ActiveTimers.Remove(TimerHandle);
				}
			}
		},
			ProcessAfterDuration, false
	);
	ActiveTimers.Add(TimerHandle);
}


UStaticMeshComponent* UTilePlanesComponent::SpawnMovementPlane(FTransform TransformForSpawn)
{
	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		FTransform Transform;
		FVector Location = TransformForSpawn.GetLocation() + FVector(0, -10.0f, 0);
		Transform.SetLocation(Location);
		
		if (UActorComponent* ActorComponent = ActorOwner->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, Transform, false))
		{
			if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponent))
			{
				FRotator Rotator = FRotator(0.0f, 00.0f, -90.0f);
				StaticMeshComponent->SetWorldRotation(Rotator);
				if (PlaneMesh)
				{
					StaticMeshComponent->SetStaticMesh(PlaneMesh);
				}
				StaticMeshComponent->RegisterComponent();
				StaticMeshComponent->SetupAttachment( ActorOwner->GetRootComponent());
				return StaticMeshComponent;
			}
		}
	}
	return nullptr;
}



void UTilePlanesComponent::AssignTileImageToMovePlane(int32 Index, UStaticMeshComponent* StaticMeshComponent)
{
	if (AActor* ActorOwner = Cast<AActor>(GetOwner()))
	{
		if (UTileInfoManagerComponent* TileInfoManagerComponent = ActorOwner->GetComponentByClass<UTileInfoManagerComponent>())
		{
			if (UMaterialInterface* MaterialInterface = TileInfoManagerComponent->TileStatuses[Index].TileInfo->Material)
			{
				StaticMeshComponent->SetMaterial(0, MaterialInterface);
			}
		}
	}
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

