// Copyright 2024 DME Games


#include "TransporterComponent.h"
#include "InteractiveActor.h"

// Sets default values for this component's properties
UTransporterComponent::UTransporterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
	PointToMoveTo = FVector::Zero();
	bArePointsSet = false;
	bCanMove = false;
	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;
	bHasBeenTriggered = false;
	ReturnsToStartPoint = false;
	SpeedPerFrame = 0.f;
}

void UTransporterComponent::SetTravelPoint(const FVector LocationChange)
{

}

// Called when the game starts
void UTransporterComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = GetOwner();

	if (!ReturnsToStartPoint)
	{
		InteractiveOwner = Cast<AInteractiveActor>(MyOwner);
	}
	
	if (MyOwner)
	{
		if (PointToMoveTo != FVector::Zero())
		{
			StartPoint = MyOwner->GetActorLocation();
			EndPoint = MyOwner->GetActorLocation() + PointToMoveTo;
			SpeedPerFrame = FVector::Distance(StartPoint, EndPoint) / MoveTime;
			bArePointsSet = true;
		}
	}
	
}

void UTransporterComponent::SetCanMove(const bool CanMove)
{
	bCanMove = CanMove;
	if (bCanMove)
	{
		bHasBeenTriggered = true;
	}
}

void UTransporterComponent::SetIsTriggered(const bool CanMove)
{
	bHasBeenTriggered = CanMove;
}

// Called every frame
void UTransporterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCanMove)
	{
		if (MyOwner && MyOwner->HasAuthority() && bArePointsSet)
		{
			FVector CurrentLocation = MyOwner->GetActorLocation();
			FVector TargetLocation = bHasBeenTriggered ? EndPoint : StartPoint;
			if (!CurrentLocation.Equals(TargetLocation))
			{
				const FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, SpeedPerFrame);
				MyOwner->SetActorLocation(NewLocation);
				if (NewLocation == EndPoint && ActivatedTriggerCount == 0)
				{
					if (!ReturnsToStartPoint)
					{
						bCanMove = false;
						if (InteractiveOwner)
						{
							InteractiveOwner->SetCanBeInteractedWith(false);
						}
					}
					else
					{
						bHasBeenTriggered = false;
					}
				}
			}
		}
	}
}

void UTransporterComponent::ChangeInOverlappingActors(const int32 NumNeededToActivate, const bool HasIncreased)
{
	if (HasIncreased)
	{
		ActivatedTriggerCount = FMath::Clamp(ActivatedTriggerCount + 1, 0, NumNeededToActivate);
		if (ActivatedTriggerCount >= NumNeededToActivate)
		{
			bCanMove = true;
			bHasBeenTriggered = true;
		}
	}
	else
	{
		ActivatedTriggerCount = FMath::Clamp(ActivatedTriggerCount - 1, 0, NumNeededToActivate);
		bHasBeenTriggered = false;
	}
}
