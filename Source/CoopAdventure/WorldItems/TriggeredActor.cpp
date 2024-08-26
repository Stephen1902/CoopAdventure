// Copyright 2024 DME Games

#include "TriggeredActor.h"
#include "InteractiveActor.h"
#include "TransporterComponent.h"
#include "CoopAdventure/Components/RotationComponent.h"

// Sets default values
ATriggeredActor::ATriggeredActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicatingMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComponent->SetupAttachment(RootComp);

	TransporterComp = CreateDefaultSubobject<UTransporterComponent>("Movement Comp");
	RotationComp = CreateDefaultSubobject<URotationComponent>("Rotation Comp");
}

// Called when the game starts or when spawned
void ATriggeredActor::BeginPlay()
{
	Super::BeginPlay();

	if (ActorsToActivate.Num() > 0)
	{
		if (HasAuthority())
		{
			for (auto& It : ActorsToActivate)
			{
				It->OnActivatedChange.AddDynamic(this, &ATriggeredActor::DealWithActivatedChanged);
			}
		}
	}
}

void ATriggeredActor::StateHasChanged(const bool NewState)
{
	OnStateChanged.Broadcast(NewState);
}

void ATriggeredActor::DealWithActivatedChanged(const bool BoolIn)
{
	if (BoolIn)
	{
		NumberActivated += 1;
		if (NumberActivated >= ActorsToActivate.Num())
		{
			BPDealWithActivatedChanged(true);
			TransporterComp->ChangeInOverlappingActors(ActorsToActivate.Num(), true);
			RotationComp->ChangeInOverlappingActors(ActorsToActivate.Num(), true);
		}
	}
	else
	{
		NumberActivated = FMath::Clamp(NumberActivated - 1, 0, ActorsToActivate.Num());
		BPDealWithActivatedChanged(false);
		TransporterComp->ChangeInOverlappingActors(0, false);
		RotationComp->ChangeInOverlappingActors(0, false);
	}
}
