// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleActor.generated.h"

UCLASS()
class COOPADVENTURE_API APuzzleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	// A list of actors that are needed to all be true to activate this actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TArray<TObjectPtr<class ATriggeredActor>> ActorsToActivate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UTransporterComponent> TransporterComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class URotatingComponent> RotatingComp;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Triggered|Triggered Events")
	void BPDealWithActivatedComplete();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32 CurrentlyActivated;

	UFUNCTION()
	void TriggeredActorChanged(const bool NewState);
};
