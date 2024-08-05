// Copyright 2024 DME Games


#include "InteractionComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	MyOwner = nullptr;
	TextToDisplay = FText::FromString("");
}


void UInteractionComponent::SetTextToDisplay(const FText NewText)
{
	if (!NewText.EqualTo(TextToDisplay))
	{
		TextToDisplay = NewText;
	}
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

