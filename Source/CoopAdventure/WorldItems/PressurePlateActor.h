// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlateActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlateActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlateDeactivated);

UCLASS()
class COOPADVENTURE_API APressurePlateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePlateActor();

	UPROPERTY(BlueprintAssignable)
	FOnPlateActivated OnPlateActivated;

	UPROPERTY(BlueprintAssignable)
	FOnPlateDeactivated OnPlateDeactivated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsActivated;
	int32 OverlappingActors;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
