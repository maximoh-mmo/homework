// Copyright 2025, Solar Corp. All right reserved

#include "InventoryAssetAppMode.h"

#include "GridInventoryEditor.h"
#include "InventoryAssetEditorApp.h"
#include "InventoryAssetPrimaryTabFactory.h"
#include "InventoryAssetPropertiesTabFactory.h"

InventoryAssetAppMode::InventoryAssetAppMode(TSharedPtr<InventoryAssetEditorApp> App)
	: FApplicationMode(GridInventory::InventoryAssetAppMode), mApp(App)
{
	// Register our main tab, if we need more tabs for this app mode we should add them here
	mTabs.RegisterFactory(MakeShareable(new InventoryAssetPrimaryTabFactory(App)));
	mTabs.RegisterFactory(MakeShareable(new InventoryAssetPropertiesTabFactory(App)));

	TabLayout = FTabManager::NewLayout("InventoryAssetAppMode_Layout_v1")->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()->SetSizeCoefficient(0.7f)->AddTab(GridInventory::InventoryAssetPrimaryTabName, ETabState::OpenedTab)
			)
			->Split
			(
			FTabManager::NewStack()->SetSizeCoefficient(0.3f)->AddTab(GridInventory::InventoryAssetPropertiesTabName, ETabState::OpenedTab)
			)
		)
	);
}

void InventoryAssetAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<InventoryAssetEditorApp> app = mApp.Pin();
	app->PushTabFactories(mTabs);
	FWorkflowCentricApplication* Application = app.Get();

	// Ensure tab manager is valid and pass as TSharedRef required by FApplicationMode API
	if (!InTabManager.IsValid())
	{
		return;
	}

	FApplicationMode::RegisterTabFactoriesWithAppAndManager(Application, InTabManager.ToSharedRef());
}

void InventoryAssetAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}

void InventoryAssetAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}
