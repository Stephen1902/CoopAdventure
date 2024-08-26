// Copyright 2024 DME Games


#include "RotationComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URotationComponent::URotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	
	StartRotation = FRotator::ZeroRotator;
	EndRotation = FRotator::ZeroRotator;
	bArePointsSet = false;
	bCanMove = true;
	MoveTime = 3.0f;
	bHasBeenTriggered = false;
	bReturnsToStartPoint = true;
	bRotationInProgress = false;
	ReturnDelay = MoveTime;
	RotationPerFrame = 0.f;
}

void URotationComponent::SetCanMove(const bool NewMove)
{
	if (!bRotationInProgress && !GetWorld()->GetTimerManager().IsTimerActive(ReturnToStartTimer))
	{
		if (GetOwner() && !GetOwner()->HasAuthority())
		{
			ServerSetCanMove(NewMove);
			return;
		}

		bHasBeenTriggered = NewMove;
	}
}

// Called when the game starts
void URotationComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = GetOwner();

	if (MyOwner && PointToMoveTo != FRotator::ZeroRotator)
	{
		StartRotation = GetOwner()->GetActorRotation();
		EndRotation = GetOwner()->GetActorRotation() + PointToMoveTo;

		if (StartRotation.Pitch != EndRotation.Pitch)
		{
			RotationPerFrame = FMath::Abs((StartRotation.Pitch - EndRotation.Pitch)) / MoveTime;
		}
		else if (StartRotation.Roll != EndRotation.Roll)
		{
			RotationPerFrame = FMath::Abs((StartRotation.Roll - EndRotation.Roll)) / MoveTime;
		}
		else
		{
			RotationPerFrame = FMath::Abs((StartRotation.Yaw - EndRotation.Yaw)) / MoveTime;
		}

		bArePointsSet = true;
	}
}

// Called every frame
void URotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCanMove)
	{
		if (MyOwner && MyOwner->HasAuthority() && bArePointsSet)
		{
			FRotator CurrentRotation = MyOwner->GetActorRotation();
			FRotator TargetRotation = bHasBeenTriggered ? EndRotation : StartRotation;
			if (!CurrentRotation.Equals(TargetRotation))
			{
				NewOwnerRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, RotationPerFrame);
				OnRep_NewRotation();
				
				if (NewOwnerRotation == EndRotation && !bReturnsToStartPoint)
				{
					bCanMove = false;
				}
			}
			else
			{
				bRotationInProgress = false;
				if (bHasBeenTriggered)
				{
					if (bReturnsToStartPoint && !GetWorld()->GetTimerManager().IsTimerActive(ReturnToStartTimer))
					{
						UE_LOG(LogTemp, Warning, TEXT("Target is at end point"));

						GetWorld()->GetTimerManager().SetTimer(ReturnToStartTimer, this, &URotationComponent::OnReturnTimerExpired, ReturnDelay, false);
					}
				}
			}
		}
	}
}

void URotationComponent::ChangeInOverlappingActors(const int32 NumNeededToActivate, const bool HasIncreased)
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


void URotationComponent::OnReturnTimerExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(ReturnToStartTimer);
	SetCanMove(false);
}

void URotationComponent::OnRep_NewRotation()
{
	if (GetOwner())
	{
		GetOwner()->SetActorRotation(NewOwnerRotation);
	}
}

void URotationComponent::ServerSetCanMove_Implementation(const bool CanMove)
{
	SetCanMove(CanMove);
}

void URotationComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URotationComponent, NewOwnerRotation);
}

