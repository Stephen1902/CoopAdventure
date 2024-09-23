// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TransporterComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPADVENTURE_API UTransporterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTransporterComponent();

	UFUNCTION(BlueprintCallable)
	void SetTravelPoint(const FVector LocationChange);

	UFUNCTION(BlueprintCallable)
	void SetCanMove(const bool CanMove);

	UFUNCTION(BlueprintCallable)
	void SetIsTriggered(const bool CanMove);

	void ChangeInOverlappingActors(const int32 NumNeededToActivate, bool HasIncreased);

	float GetMovementTime() const { return MoveTime; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Time in seconds to move from one point to the other
	UPROPERTY(EditAnywhere, Category = "Set Up")
	float MoveTime;

	// Whether the owning actor returns to StartPoint when deactivated
	UPROPERTY(EditAnywhere, Category = "Set Up")
	bool ReturnsToStartPoint;

	// Relative point to move to when activated
	UPROPERTY(EditAnywhere, Category = "Set Up")
	FVector PointToMoveTo;
	
	// Whether this component has been triggered
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bHasBeenTriggered;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector StartPoint;
	FVector EndPoint;
	
	bool bArePointsSet;

	UPROPERTY()
	AActor* MyOwner;

	UPROPERTY()
	class AInteractiveActor* InteractiveOwner;

	float SpeedPerFrame;
	bool bCanMove;
	
	// Number of TriggerActors that have been triggered
	int32 ActivatedTriggerCount;

};
