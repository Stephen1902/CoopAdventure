// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "CoopAdventure/Framework/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivatedChange, bool, NewBoolState);

UCLASS()
class COOPADVENTURE_API AInteractiveActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

	UPROPERTY()
	FOnActivatedChange OnActivatedChange;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	class UInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	class UTransporterComponent* TransporterComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	class URotationComponent* RotationComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FText LookAt_Implementation() override;
	UFUNCTION()
	virtual void InteractWith_Implementation(ATP_FirstPersonCharacter* CharacterWhoInteracted) override;
};
