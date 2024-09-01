// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetCompWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOPADVENTURE_API UWidgetCompWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up", meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> WidgetPanel; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up", meta=(BindWidget))
	TObjectPtr<class UTextBlock> DisplayTextBlock;

	// Text to show on screen when the owner of this widget is viewed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	FText TextToDisplay;
};
