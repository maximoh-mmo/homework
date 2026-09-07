// Copyright 2025, Solar Corp. All right reserved


#include "Interfaces/GridItemOwner.h"


TArray<TSubclassOf<UGridItemActionBase>> IGridItemOwner::GetOwnerActionClasses_Implementation() const
{
	return {};
}
