// Copyright 2024 DME Games


#include "TransporterComponent.h"

#include "PressurePlateActor.h"

// Sets default values for this component's properties
UTransporterComponent::UTransporterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
	bArePointsSet = false;
	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;
	bHasBeenTriggered = false;
	ReturnsToStartPoint = false;
	SpeedPerFrame = 0.f;
}


// Called when the game starts
void UTransporterComponent::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* TriggerActor : TriggerActors)
	{
		if (APressurePlateActor* PressurePlateActor = Cast<APressurePlateActor>(TriggerActor))
		{
			PressurePlateActor->OnPlateActivated.AddDynamic(this, &UTransporterComponent::OnPressurePlateActivated);
			PressurePlateActor->OnPlateDeactivated.AddDynamic(this, &UTransporterComponent::OnPressurePlateDeactivated);
		}
	}

	MyOwner = GetOwner();
}

void UTransporterComponent::SetTravelPoints(const FVector Point1, const FVector Point2)
{
	if (!Point1.Equals(Point2))
	{
		StartPoint = Point1;
		EndPoint = Point2;
		SpeedPerFrame = FVector::Distance(StartPoint, EndPoint) / MoveTime;
		bArePointsSet = true;
	}
}

// Called every frame
void UTransporterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MyOwner && MyOwner->HasAuthority() && bArePointsSet)
	{
		FVector CurrentLocation = MyOwner->GetActorLocation();
		FVector TargetLocation = bHasBeenTriggered ? EndPoint : StartPoint;
		if (!CurrentLocation.Equals(TargetLocation) || (CurrentLocation.Equals(EndPoint) && ReturnsToStartPoint))
		{
			const FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, SpeedPerFrame);
			MyOwner->SetActorLocation(NewLocation);
		}
	}
}

void UTransporterComponent::OnPressurePlateActivated()
{
	ActivatedTriggerCount++;
	if (ActivatedTriggerCount >= TriggerActors.Num() && !bHasBeenTriggered)
	{
		bHasBeenTriggered = true;
	}
}

void UTransporterComponent::OnPressurePlateDeactivated()
{
	ActivatedTriggerCount--;
	if (ActivatedTriggerCount < TriggerActors.Num() && bHasBeenTriggered)
	{
		bHasBeenTriggered = false;
	}
}
