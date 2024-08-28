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
	
protected:
	virtual void BeginPlay() override;

	//  Amount of time the owner of this component moves for when active
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	float MovementTime;

	// Time delay before returning the owner of this component to it's original position.  0 if never returns
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	float ReturnDelay;
private:
	UFUNCTION()
	void OnActivated(UActorComponent* Component, bool Reset);

	UFUNCTION()
	void OnDeactivated(UActorComponent* Component);

	FTimerHandle RotationStopTimer;
	FTimerHandle RotationReturnTimer;

	UFUNCTION()
	void RotationTimerHasExpired();

	UFUNCTION()
	void ReturnTimerHasExpired();

	bool bCanReturn;
};
