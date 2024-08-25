// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "LineTraceInteractive.generated.h"

/**
 * 
 */
UCLASS()
class COOPADVENTURE_API ALineTraceInteractive : public AInteractiveActor
{
	GENERATED_BODY()

	ALineTraceInteractive();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UInteractionComponent* InteractionComp;

	virtual FText LookAt_Implementation() override;

	UFUNCTION()
	virtual void InteractWith_Implementation(ATP_FirstPersonCharacter* CharacterWhoInteracted) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* WidgetComp;
};
