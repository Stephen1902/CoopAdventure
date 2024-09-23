// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "RotatingComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPADVENTURE_API URotatingComponent : public URotatingMovementComponent
{
	GENERATED_BODY()

	URotatingComponent();
public:
	UFUNCTION(BlueprintCallable)
	void ReactToActivationChange(const bool NewState);

	float GetMovementTime() const { return MovementTime; }
protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	virtual void BeginPlay() override;

	//  Amount of time the owner of this component moves for when active
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	float MovementTime;

	// Whether the owning actor should return to it's start point when deactivated ie when a player steps off a trigger
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool bReturnsWhenDeactivated;

	// Time delay before returning the owner of this component to it's original position.  0 if never returns.  Only available if doesn't automatically return when deactivated.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables", meta=(EditCondition="!bReturnsWhenDeactivated"))
	float ReturnDelay;

private:
	FRotator StartingRotation;
	
	UFUNCTION()
	void OnActivated(UActorComponent* Component, bool Reset);

	UFUNCTION()
	void OnDeactivated(UActorComponent* Component);

	FTimerHandle RotationStopTimer;
	FTimerHandle RotationReturnTimer;
	FTimerHandle DeactivationCheckTimer;

	UFUNCTION()
	void RotationTimerHasExpired();

	UFUNCTION()
	void ReturnTimerHasExpired();

	UFUNCTION()
	void DeactivatedCheckTimer();

	bool bCanReturn;

	bool bIsDeactivated;
};
