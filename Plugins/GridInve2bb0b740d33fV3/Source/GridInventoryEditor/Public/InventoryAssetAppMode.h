// Copyright 2025, Solar Corp. All right reserved
#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class InventoryAssetEditorApp;
class FTabManager;

// We can have several modes but for inventory asset we need only one for now
class InventoryAssetAppMode : public FApplicationMode
{
public:
	InventoryAssetAppMode(TSharedPtr<InventoryAssetEditorApp> App);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PostActivateMode() override;
	virtual void PreDeactivateMode() override;

private:
	TWeakPtr<InventoryAssetEditorApp> mApp;
	FWorkflowAllowedTabSet mTabs;
};
