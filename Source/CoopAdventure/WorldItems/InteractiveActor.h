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

	UFUNCTION(BlueprintCallable, Category = "Interactive|Interactive")
	void SetCanBeInteractedWith(const bool InteractionState);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UTransporterComponent* TransporterComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class URotatingComponent* RotatingComp;

	bool bCanBeInteractedWith;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
