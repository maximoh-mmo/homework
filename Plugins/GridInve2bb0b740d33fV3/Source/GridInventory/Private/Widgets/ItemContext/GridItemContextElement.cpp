// Copyright 2025, Solar Corp. All right reserved

#include "Widgets/ItemContext/GridItemContextElement.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UGridItemContextElement::NativeConstruct()
{
	Super::NativeConstruct();

	ContextButton->OnClicked.AddDynamic(this, &UGridItemContextElement::OnButtonClicked);
}

void UGridItemContextElement::NativeDestruct()
{
	ContextButton->OnClicked.RemoveDynamic(this, &UGridItemContextElement::OnButtonClicked);
	
	Super::NativeDestruct();
}

void UGridItemContextElement::SetupElement(FText InActionName)
{
	ContextNameTextBlock->SetText(InActionName);	
}

void UGridItemContextElement::OnButtonClicked()
{
	if (OnElementClicked.IsBound())
	{
		OnElementClicked.Broadcast(this);
	}
}
