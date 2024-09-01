// Copyright 2024 DME Games

#include "LineTraceInteractive.h"
#include "TransporterComponent.h"
#include "CoopAdventure/Components/InteractionComponent.h"
#include "CoopAdventure/Components/RotatingComponent.h"

ALineTraceInteractive::ALineTraceInteractive()
{
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>("Interaction Comp");
}

FText ALineTraceInteractive::LookAt_Implementation()
{
	if (bCanBeInteractedWith)
	{
		if (InteractionComp)
		{
			if (!InteractionComp->GetTextToDisplay().IsEmpty())
			{
				return InteractionComp->GetTextToDisplay();
			}
		}
	}
	
	return FText::FromString("");
}

void ALineTraceInteractive::InteractWith_Implementation(ATP_FirstPersonCharacter* CharacterWhoInteracted)
{
	Super::InteractWith_Implementation(CharacterWhoInteracted);

	if (bCanBeInteractedWith)
	{
		TransporterComp->SetCanMove(true);
		RotatingComp->ReactToActivationChange(true);

		OnActivatedChange.Broadcast(true);
		
		IInteractionInterface::InteractWith_Implementation(CharacterWhoInteracted);
	}
}
