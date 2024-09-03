// Copyright 2024 DME Games

#include "RotatingComponent.h"

URotatingComponent::URotatingComponent()
{
	bRotationInLocalSpace = true;
	MovementTime = 1.0f;
	ReturnDelay = 0.f;
	bCanReturn = false;
	bReturnsWhenDeactivated = false;
	bIsDeactivated = true;
	StartingRotation = FRotator::ZeroRotator;
	
	bAutoActivate = false;
}

void URotatingComponent::ReactToActivationChange(const bool NewState)
{
	if (bReturnsWhenDeactivated)
	{
		bCanReturn = NewState;
		if (!bCanReturn && GetWorld()->GetTimerManager().IsTimerActive(RotationStopTimer))
		{
			const float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(RotationStopTimer);
			GetWorld()->GetTimerManager().ClearTimer(RotationStopTimer);
			RotationRate = StartingRotation * -1.f;
			GetWorld()->GetTimerManager().SetTimer(RotationStopTimer, this, &URotatingComponent::RotationTimerHasExpired, TimeElapsed, false);
		}
	}
	
	if (!GetWorld()->GetTimerManager().IsTimerActive(RotationStopTimer) && !GetWorld()->GetTimerManager().IsTimerActive(RotationReturnTimer))
	{
		if (NewState)
		{
			RotationRate = StartingRotation;
		}
		SetActive(NewState);
	}
}

#if WITH_EDITOR
void URotatingComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (bReturnsWhenDeactivated)
	{
		ReturnDelay = 0.f;
	}
}
#endif

void URotatingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentActivated.AddDynamic(this, &URotatingComponent::OnActivated);
	OnComponentDeactivated.AddDynamic(this, &URotatingComponent::OnDeactivated);

	StartingRotation = RotationRate;
}

void URotatingComponent::OnActivated(UActorComponent* Component, bool Reset)
{
	GetWorld()->GetTimerManager().SetTimer(RotationStopTimer, this, &URotatingComponent::RotationTimerHasExpired, MovementTime, false);
}

void URotatingComponent::OnDeactivated(UActorComponent* Component)
{
	if (ReturnDelay > 0.f)
	{
		bCanReturn = !bCanReturn;

		if (bCanReturn)
		{
			RotationRate = StartingRotation * -1.f;
			GetWorld()->GetTimerManager().SetTimer(DeactivationCheckTimer, this, &URotatingComponent::ReturnTimerHasExpired, ReturnDelay, false);
		}
	}
}

void URotatingComponent::RotationTimerHasExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(RotationStopTimer);
	SetActive(false);
	
	if (bReturnsWhenDeactivated && bCanReturn)
	{
		GetWorld()->GetTimerManager().SetTimer(DeactivationCheckTimer, this, &URotatingComponent::DeactivatedCheckTimer, 0.05f, true);
	}
	else
	{
		if (bReturnsWhenDeactivated)
		{
			RotationRate = StartingRotation;
			bCanReturn = true;
		}
	}
}

void URotatingComponent::ReturnTimerHasExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(RotationReturnTimer);
	SetActive(true);
}

void URotatingComponent::DeactivatedCheckTimer()
{
	if (!bCanReturn)
	{
		if (RotationRate != StartingRotation * -1.f)
		{
			RotationRate = StartingRotation * -1.f;
			SetActive(true);
		}
	}
}
