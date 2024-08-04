// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatformActor.generated.h"

UCLASS()
class COOPADVENTURE_API AMovingPlatformActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatformActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UArrowComponent> StartPoint;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UArrowComponent> EndPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UTransporterComponent* TransporterComp; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
