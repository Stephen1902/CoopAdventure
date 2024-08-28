// Copyright 2024 DME Games

#include "RotatingComponent.h"

URotatingComponent::URotatingComponent()
{
	bRotationInLocalSpace = true;
	MovementTime = 1.0f;
	ReturnDelay = 0.f;
	bCanReturn = false;
	
	bAutoActivate = false;
}

void URotatingComponent::ReactToActivationChange(const bool NewState)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(RotationStopTimer) && !GetWorld()->GetTimerManager().IsTimerActive(RotationReturnTimer))
	{
		SetActive(NewState);
	}
}

void URotatingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentActivated.AddDynamic(this, &URotatingComponent::OnActivated);
	OnComponentDeactivated.AddDynamic(this, &URotatingComponent::OnDeactivated);
	
}

void URotatingComponent::OnActivated(UActorComponent* Component, bool Reset)
{
	GetWorld()->GetTimerManager().SetTimer(RotationStopTimer, this, &URotatingComponent::RotationTimerHasExpired, MovementTime, false);
}

void URotatingComponent::OnDeactivated(UActorComponent* Component)
{
	if (ReturnDelay > 0.f)
	{
		RotationRate = RotationRate * -1.f;

		bCanReturn = !bCanReturn;

		if (bCanReturn)
		{
			GetWorld()->GetTimerManager().SetTimer(RotationReturnTimer, this, &URotatingComponent::ReturnTimerHasExpired, ReturnDelay, false);
		}
	}
}

void URotatingComponent::RotationTimerHasExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(RotationStopTimer);
	ReactToActivationChange(false);
}

void URotatingComponent::ReturnTimerHasExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(RotationReturnTimer);
	ReactToActivationChange(true);
}
