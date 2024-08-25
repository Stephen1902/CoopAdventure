// Copyright 2024 DME Games

#include "LineTraceInteractive.h"
#include "TransporterComponent.h"
#include "Components/WidgetComponent.h"
#include "CoopAdventure/Components/InteractionComponent.h"
#include "CoopAdventure/Components/RotationComponent.h"

ALineTraceInteractive::ALineTraceInteractive()
{
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>("Interaction Comp");

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("Widget Comp");
	WidgetComp->SetVisibility(false);
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
		

		/*	if (WidgetComp)
			{
				WidgetComp->SetVisibility(true);
			}
			*/
	}
	
	return FText::FromString("");
}

void ALineTraceInteractive::InteractWith_Implementation(ATP_FirstPersonCharacter* CharacterWhoInteracted)
{
	Super::InteractWith_Implementation(CharacterWhoInteracted);

	if (bCanBeInteractedWith)
	{
		TransporterComp->SetCanMove(true);
		RotationComp->SetCanMove(true);

		OnActivatedChange.Broadcast(true);
	
		IInteractionInterface::InteractWith_Implementation(CharacterWhoInteracted);
	}
}
