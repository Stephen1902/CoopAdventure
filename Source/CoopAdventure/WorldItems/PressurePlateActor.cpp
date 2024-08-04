// Copyright 2024 DME Games


#include "PressurePlateActor.h"
#include "Components/SphereComponent.h"

// Sets default values
APressurePlateActor::APressurePlateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Mesh"));
	CollisionComp->SetupAttachment(RootComp);
	CollisionComp->SetIsReplicated(true);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComp);
	
	bIsActivated = false;
	OverlappingActors = 0;
}

// Called when the game starts or when spawned
void APressurePlateActor::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APressurePlateActor::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &APressurePlateActor::OnEndOverlap);
}

// Called every frame
void APressurePlateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APressurePlateActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("TriggerActor"))
	{
		OnPlateActivated.Broadcast();
	}
}

void APressurePlateActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("TriggerActor"))
	{
		OnPlateDeactivated.Broadcast();
	}
}

