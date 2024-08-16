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
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetTravelPoints(const FRotator RotPoint1, const FRotator RotPoint2);

	// Time in seconds to move from one point to the other
	UPROPERTY(EditAnywhere)
	float MoveTime;
	
	// Whether or not the owning actor returns to StartPoint when deactivated
	UPROPERTY(EditAnywhere)
	bool bReturnsToStartPoint;

	// How long before this item returns to it's original location
	UPROPERTY(EditAnywhere, meta=(EditCondition="bReturnsToStartPoint"))
	float ReturnDelay;
	
	// Whether or not this component has been triggered
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
};
