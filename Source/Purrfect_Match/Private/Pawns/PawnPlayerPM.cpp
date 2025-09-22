// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/PawnPlayerPM.h"

#include "EnhancedInputSubsystems.h"
#include "K2Node_GetSubsystem.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Binding/DelegateBindingCompPlayerPawn.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameBoard/GameBoard.h"
#include "Logging/StructuredLog.h"

// Sets default values
APawnPlayerPM::APawnPlayerPM()
{
	SceneComponentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentRoot"));
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	DelegateBindingCompPlayerPawn = CreateDefaultSubobject<UDelegateBindingCompPlayerPawn>(TEXT("DelegateBindingCompPlayerPawn"));
	
	SetRootComponent(SceneComponentRoot);
	StaticMesh->SetupAttachment(SceneComponentRoot);

	SceneComponentRoot->SetMobility(EComponentMobility::Type::Movable);
	StaticMesh->SetMobility(EComponentMobility::Type::Movable);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void APawnPlayerPM::BeginPlay()
{
	Super::BeginPlay();
	
}

void APawnPlayerPM::MoveHorizontal(const FInputActionValue& InputActionValue)
{
	UE_LOGFMT(LogTemp, Warning, "side player");
	const float axisValue = InputActionValue.Get<float>();
	if (axisValue < 0)
	{
		if (indexRight <= 0)
		{
			indexRight=0;
			indexLeft=1;
			return;
		}
		if (indexRight  % boardWidth == 0)
		{
			indexLeft += axisValue * 2;
			indexRight += axisValue * 2;
			DelegateBindingCompPlayerPawn->GameStatePM->PawnPlayerRequestTileLocationDelegate.Broadcast(indexLeft);
			return;
		}
	}

	if (axisValue > 0)
	{
		if (indexLeft >=  lastIndex)
		{
			indexRight = lastIndex - 1;
			indexLeft = lastIndex;
			return;
		}
		if ((indexLeft + 1)  % boardWidth == 0)
		{
			indexLeft += axisValue * 2;
			indexRight += axisValue * 2;
			DelegateBindingCompPlayerPawn->GameStatePM->PawnPlayerRequestTileLocationDelegate.Broadcast(indexLeft);
			return;
		}
	}
	
	{
		indexLeft += axisValue;
		indexRight += axisValue;
	}
	
	DelegateBindingCompPlayerPawn->GameStatePM->PawnPlayerRequestTileLocationDelegate.Broadcast(indexLeft);
}

void APawnPlayerPM::MoveVertical(const FInputActionValue& InputActionValue)
{
	const float axisValue = InputActionValue.Get<float>();
	if (axisValue < 0)
	{
		if (indexRight <= (boardWidth-1) || indexLeft <= (boardWidth-1))
		{
			return;
		}
	}

	if (axisValue > 0)
	{
		if (indexRight >= firstIndexOfLastRow || indexLeft >= firstIndexOfLastRow)
		{
			return;
		}
	}
	if (axisValue < 0)
	{
		if (indexRight <= boardWidth -1 || indexLeft <= boardWidth - 1)
		{
			return;
		}
	}
	indexLeft += axisValue * (boardWidth);
	indexRight += axisValue * (boardWidth);
	DelegateBindingCompPlayerPawn->GameStatePM->PawnPlayerRequestTileLocationDelegate.Broadcast(indexLeft);
	
}

void APawnPlayerPM::SwitchTiles(const FInputActionValue& InputActionValue)
{
	DelegateBindingCompPlayerPawn->GameStatePM->GameBoardSwitchTilesDelegate.Broadcast(indexLeft, indexRight);
}

// Called every frame
void APawnPlayerPM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnPlayerPM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOGFMT(LogTemp, Warning, "SetupInputComponent player");
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsValid(PlayerInputComponent))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					if (IsValid(EnhancedInputLocalPlayerSubsystem))
					{
						EnhancedInputLocalPlayerSubsystem->AddMappingContext(PlayerInputMappingContext, 0);
					}

					if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
					{
						if (IsValid(IAMoveHorizontal))
						{
							EnhancedInputComponent->BindAction(IAMoveHorizontal, ETriggerEvent::Triggered, this, &APawnPlayerPM::MoveHorizontal);
						}
						if (IsValid(IAMoveVertical))
						{
							EnhancedInputComponent->BindAction(IAMoveVertical, ETriggerEvent::Triggered, this, &APawnPlayerPM::MoveVertical);
						}

						if (IsValid(IAMoveVertical))
						{
							EnhancedInputComponent->BindAction(IASwitchTiles, ETriggerEvent::Triggered, this, &APawnPlayerPM::SwitchTiles);
						}
					}
				}
			}
		}
	}
}

void APawnPlayerPM::SetStartLocation()
{
	DelegateBindingCompPlayerPawn->GameStatePM->PawnPlayerRequestTileLocationDelegate.Broadcast(indexLeft);
}

void APawnPlayerPM::SetPawnLocation(FVector Location)
{
	SetActorLocation(Location);
}

void APawnPlayerPM::SetBoardWithAndHeight(int32 InBoardWidth, int32 InHeight)
{
	boardWidth = InBoardWidth;
	boardHeight = InHeight;
	lastIndex = (boardHeight * boardWidth) - 1;
	firstIndexOfLastRow = lastIndex - boardWidth + 1;
}



