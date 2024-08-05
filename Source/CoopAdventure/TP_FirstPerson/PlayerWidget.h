// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOPADVENTURE_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMessageText(const FText& TextIn);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* MessageText;

private:
	virtual void NativePreConstruct() override;
};
