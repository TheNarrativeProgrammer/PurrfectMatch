// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Binding/DelegateBindingComponent.h"

// Sets default values for this component's properties
UDelegateBindingComponent::UDelegateBindingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UDelegateBindingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDelegateBindingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (AGameStatePM* GameState = Cast<AGameStatePM>(GetWorld()->GetGameState()))
	{
		GameStatePM = GameState;
	}
	BindDelegates();
}

void UDelegateBindingComponent::BindDelegates()
{
}


// Called every frame
void UDelegateBindingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

