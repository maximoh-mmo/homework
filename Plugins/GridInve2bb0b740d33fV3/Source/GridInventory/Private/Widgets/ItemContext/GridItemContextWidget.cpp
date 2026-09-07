// Copyright 2025, Solar Corp. All right reserved


#include "Widgets/ItemContext/GridItemContextWidget.h"
#include "Widgets/ItemContext/GridItemContextElement.h"
#include "GridItem.h"
#include "Actions/GridItemActionBase.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Interfaces/GridItemOwner.h"


void UGridItemContextWidget::SetupContext(UGridItem* InItem)
{
	if (!ensureAlwaysMsgf(IsValid(ContextElementClass), TEXT("ContextElementClass is invalid, please assign a valid class")))
	{
		return;
	}

	Item = InItem;
	for (const TSubclassOf<UGridItemActionBase> actionClass : InItem->ItemActionClasses)
	{
		UGridItemActionBase* action = NewObject<UGridItemActionBase>(this, actionClass);
		Actions.Add(action);
	}

	TArray<TSubclassOf<UGridItemActionBase>> actionClasses = IGridItemOwner::Execute_GetOwnerActionClasses(InItem->GetItemOwnerInterfaceObject());
	for (const TSubclassOf<UGridItemActionBase> actionClass : actionClasses)
	{
		UGridItemActionBase* action = NewObject<UGridItemActionBase>(this, actionClass);
		Actions.Add(action);
	}

	for (int32 i = 0; i < Actions.Num(); ++i)
	{
		UGridItemActionBase* action = Actions[i];
		if (!ensureAlwaysMsgf(IsValid(action), TEXT("Invalid action")))
		{
			return;
		}

		const TSubclassOf<UGridItemContextElement>& contextClass = IsValid(action->ActionContextElementClass) ? action->ActionContextElementClass : ContextElementClass;
		
		UGridItemContextElement* contextElement = CreateWidget<UGridItemContextElement>(GetOwningPlayer(), contextClass);
		contextElement->SetupElement(action->ActionName);
		contextElement->OnElementClicked.AddUniqueDynamic(this, &UGridItemContextWidget::OnActionElementClicked);
		
		UVerticalBoxSlot* elementAsBoxSlot = ElementsBox->AddChildToVerticalBox(contextElement);

		// first element doesn't need top padding
		if (i == 0)
		{
			continue;
		}
		
		elementAsBoxSlot->SetPadding(ElementPadding);
	}
}

void UGridItemContextWidget::OnActionElementClicked(UGridItemContextElement* InClickedElement)
{
	int32 clickedIndex = ElementsBox->GetChildIndex(InClickedElement); 

	if (!ensureAlwaysMsgf(clickedIndex != INDEX_NONE, TEXT("Clicked on Invalid index")))
	{
		return;
	}

	FGridActionContext actionContext;
	actionContext.Item = Item;
	actionContext.ContextWidget = this;
	actionContext.InstigatorPawn = GetOwningPlayerPawn();
	actionContext.InstigatorController = GetOwningPlayer();
	Actions[clickedIndex]->ExecuteAction(actionContext);
}
