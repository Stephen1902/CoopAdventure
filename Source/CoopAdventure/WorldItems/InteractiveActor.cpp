// Copyright 2024 DME Games


#include "InteractiveActor.h"

#include "TransporterComponent.h"
#include "CoopAdventure/Components/InteractionComponent.h"
#include "CoopAdventure/TP_FirstPerson/TP_FirstPersonCharacter.h"

#define TRACE_INTERACTIVE ECC_GameTraceChannel1

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(true);
	
	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetIsReplicated(true);
	MeshComp->SetCollisionResponseToChannel(TRACE_INTERACTIVE, ECR_Block);  // Block line traces looking for the interactive channel

	InteractionComp = CreateDefaultSubobject<UInteractionComponent>("Interaction Comp");
	TransporterComp = CreateDefaultSubobject<UTransporterComponent>("Transporter Comp");
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner())
	{
		GetOwner()->SetReplicates(true);
	}
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText AInteractiveActor::LookAt_Implementation()
{
	if (InteractionComp)
	{
		if (!InteractionComp->GetTextToDisplay().IsEmpty())
		{
			return InteractionComp->GetTextToDisplay();
		}
	}
	
	return FText::FromString("");
}

void AInteractiveActor::InteractWith_Implementation(ATP_FirstPersonCharacter* CharacterWhoInteracted)
{
	IInteractionInterface::InteractWith_Implementation(CharacterWhoInteracted);

	UE_LOG(LogTemp, Warning, TEXT("C++ InteractWith_Implementation called."));
}

