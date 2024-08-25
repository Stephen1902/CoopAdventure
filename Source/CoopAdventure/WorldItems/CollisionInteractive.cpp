// Copyright 2024 DME Games

#include "CollisionInteractive.h"

#include "TransporterComponent.h"
#include "Components/SphereComponent.h"

ACollisionInteractive::ACollisionInteractive()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>("Collision Comp");
	CollisionComp->SetupAttachment(RootComp);
	CollisionComp->SetIsReplicated(true);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ACollisionInteractive::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComp)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACollisionInteractive::OnBeginOverlap);
		CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ACollisionInteractive::OnEndOverlap);
	}
}

void ACollisionInteractive::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnActivatedChange.Broadcast(true);

	if (TransporterComp)
	{
		TransporterComp->SetCanMove(true);
	}
}

void ACollisionInteractive::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnActivatedChange.Broadcast(false);
}
