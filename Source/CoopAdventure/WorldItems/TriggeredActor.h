// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeredActor.generated.h"

/** A base class for any actor that is only activated by another actor triggering it ie, a door that needs a code entered into it by an interactive actor */

UCLASS()
class COOPADVENTURE_API ATriggeredActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggeredActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	// A list of actors that are needed to all be true to activate this actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TArray<TObjectPtr<class AInteractiveActor>> ActorsToActivate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UTransporterComponent> TransporterComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class URotationComponent> RotationComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32 NumberActivated;

	UFUNCTION()
	void DealWithActivatedChanged(const bool BoolIn);
};