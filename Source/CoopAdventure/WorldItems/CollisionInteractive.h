// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "CollisionInteractive.generated.h"

/**
 * 
 */
UCLASS()
class COOPADVENTURE_API ACollisionInteractive : public AInteractiveActor
{
	GENERATED_BODY()

	ACollisionInteractive();

protected:
	// Called when game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	TObjectPtr<class USphereComponent> CollisionComp;

private:
	UFUNCTION()
   	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    
   	UFUNCTION()
   	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
