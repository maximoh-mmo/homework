// Copyright 2025, Solar Corp. All right reserved

#include "InventoryAssetPropertiesTabFactory.h"

#include "GridInventoryEditor.h"
#include "InventoryAssetEditorApp.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

InventoryAssetPropertiesTabFactory::InventoryAssetPropertiesTabFactory(TSharedPtr<InventoryAssetEditorApp> App)
	: FWorkflowTabFactory(GridInventory::InventoryAssetPropertiesTabName, App), mApp(App)
{
	TabLabel = FText::FromString(TEXT("Properties"));
	ViewMenuDescription = FText::FromString(TEXT("Displays properties for inventory asset"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the properties view"));
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");
}

TSharedRef<SWidget> InventoryAssetPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<InventoryAssetEditorApp> app = mApp.Pin();
	return app->CreatePropertiesWidget();
}

FText InventoryAssetPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A property view for the current asset"));
}
