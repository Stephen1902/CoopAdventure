// Copyright 2024 DME Games

#include "PlayerWidget.h"
#include "Components/TextBlock.h"

void UPlayerWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    //SetMessageText(FText::FromString(""));
}

void UPlayerWidget::SetMessageText(const FText& TextIn)
{
    if (TextIn.IsEmpty())
    {
        MessageText->SetVisibility(ESlateVisibility::Hidden);	
    }
    else
    {
        MessageText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        MessageText->SetText(TextIn);
    }
}
