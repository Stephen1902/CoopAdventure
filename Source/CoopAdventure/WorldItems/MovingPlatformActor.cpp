// Copyright 2024 DME Games

#include "MovingPlatformActor.h"
#include "TransporterComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AMovingPlatformActor::AMovingPlatformActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComp->SetIsReplicated(true);
	MeshComp->SetupAttachment(RootComp);

	StartPoint = CreateDefaultSubobject<UArrowComponent>("Start Point");
	StartPoint->SetupAttachment(MeshComp);

	EndPoint = CreateDefaultSubobject<UArrowComponent>("End Point");
	EndPoint->SetupAttachment(MeshComp);

	TransporterComp = CreateDefaultSubobject<UTransporterComponent>(TEXT("Transporter Comp"));
}

// Called when the game starts or when spawned
void AMovingPlatformActor::BeginPlay()
{
	Super::BeginPlay();

	if (TransporterComp)
	{
		TransporterComp->SetTravelPoints(GetActorLocation() + StartPoint->GetRelativeLocation(), GetActorLocation() + EndPoint->GetRelativeLocation());
	}
}

// Called every frame
void AMovingPlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

