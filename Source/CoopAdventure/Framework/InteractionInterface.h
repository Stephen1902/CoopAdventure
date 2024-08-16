// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COOPADVENTURE_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactive")
	FText LookAt();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactive")
	void InteractWith(class ATP_FirstPersonCharacter* CharacterWhoInteracted);
};
