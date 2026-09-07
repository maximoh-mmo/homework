// Copyright 2025, Solar Corp. All right reserved

#include "Data/GridPayload.h"

#include "GridItem.h"

void UGridPayload::ResetItemInitialRotation()
{
	GridItem->SetIsRotated(bIsItemInitiallyRotated);
}

void UGridPayload::SetItemRotation(bool InRotation)
{
	GridItem->SetIsRotated(InRotation);
}
