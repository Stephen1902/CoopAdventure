// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPADVENTURE_API URotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotationComponent();

	UFUNCTION(BlueprintCallable)
	void SetCanMove(const bool NewMove);

	void ChangeInOverlappingActors(const int32 NumNeededToActivate, const bool HasIncreased);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Time in seconds to move from one point to the other
	UPROPERTY(EditAnywhere, Category = "Set Up")
	float MoveTime;
	
	// Whether the owning actor returns to StartPoint when deactivated
	UPROPERTY(EditAnywhere, Category = "Set Up")
	bool bReturnsToStartPoint;

	// How long before this item returns to it's original location
	UPROPERTY(EditAnywhere, meta=(EditCondition="bReturnsToStartPoint"), Category = "Set Up")
	float ReturnDelay;

	UPROPERTY(EditAnywhere, Category = "Set Up")
	FRotator PointToMoveTo;
	
	// Whether this component has been triggered
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bHasBeenTriggered;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FRotator StartRotation;
	FRotator EndRotation;
	bool bRotationInProgress;
	bool bArePointsSet;
	
	UPROPERTY()
	AActor* MyOwner;

	float RotationPerFrame;
	bool bCanMove;

	FTimerHandle ReturnToStartTimer;
	UFUNCTION()
	void OnReturnTimerExpired();

	UFUNCTION(Server, Reliable)
	void ServerSetCanMove(const bool CanMove);

	UPROPERTY(ReplicatedUsing=OnRep_NewRotation)
	FRotator NewOwnerRotation;

	UFUNCTION()
	void OnRep_NewRotation();
	
	// Number of TriggerActors that have been triggered
	int32 ActivatedTriggerCount;
};
