// Copyright 2025, Solar Corp. All right reserved


#include "Actions/GridItemActionBase.h"

bool UGridItemActionBase::ExecuteAction(const FGridActionContext& ActionContext)
{
	return ExecuteActionInternal(ActionContext);
}

bool UGridItemActionBase::ExecuteActionInternal(const FGridActionContext& ActionContext)
{
	return true;
}

bool UGridItemActionBP::ExecuteActionInternal(const FGridActionContext& ActionContext)
{
	return BP_ExecuteAction(ActionContext);
}

bool UGridItemActionBP::BP_ExecuteAction_Implementation(const FGridActionContext& ActionContext)
{
	return true;
}