// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class InventoryAssetEditorApp;


class InventoryAssetPropertiesTabFactory : public FWorkflowTabFactory
{
public:
	InventoryAssetPropertiesTabFactory(TSharedPtr<InventoryAssetEditorApp> App);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<InventoryAssetEditorApp> mApp;
};
