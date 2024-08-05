// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPADVENTURE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	FText GetTextToDisplay() const { return TextToDisplay; }
	void SetTextToDisplay(const FText NewText);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// The text to be shown on the player screen when they are looking at this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText TextToDisplay;

private:
	UPROPERTY()
	AActor* MyOwner;
		
};
