// Copyright 2024 DME Games


#include "PuzzleActor.h"

#include "TransporterComponent.h"
#include "TriggeredActor.h"
#include "CoopAdventure/Components/RotationComponent.h"

// Sets default values
APuzzleActor::APuzzleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComponent->SetupAttachment(RootComp);

	TransporterComp = CreateDefaultSubobject<UTransporterComponent>("Movement Comp");
	RotationComp = CreateDefaultSubobject<URotationComponent>("Rotation Comp");
	
	CurrentlyActivated = 0;
}

// Called when the game starts or when spawned
void APuzzleActor::BeginPlay()
{
	Super::BeginPlay();

	if (ActorsToActivate.Num() > 0 && HasAuthority())
	{
		for (auto& It : ActorsToActivate)
		{
			It->OnStateChanged.AddDynamic(this, &APuzzleActor::TriggeredActorChanged);	
		}
	}
}

// Called every frame
void APuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleActor::TriggeredActorChanged(const bool NewState)
{
	if (NewState)
	{
		CurrentlyActivated = FMath::Clamp(CurrentlyActivated + 1, 0, ActorsToActivate.Num());

		UE_LOG(LogTemp, Warning, TEXT("NumNeeded %i, ArrayNum %i"), CurrentlyActivated, ActorsToActivate.Num());
		//  Check if the total actors needed to activate has been activated then do something
		if (CurrentlyActivated >= ActorsToActivate.Num())
		{
			BPDealWithActivatedComplete();
		}
	}
	else
	{
		CurrentlyActivated = FMath::Clamp(CurrentlyActivated - 1, 0, ActorsToActivate.Num());
	}
}

