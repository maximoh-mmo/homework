// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UInventoryAsset;
class IToolkitHost;
class SWidget;
class SOverlay;
class UInventorySlotWrapper;
class IDetailsView;

struct FPropertyChangedEvent;

class InventoryAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	~InventoryAssetEditorApp();
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override;

	// FEditorUndoClient
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	void HandleUndoRedo();
	
	// FNotifyHook
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;
	
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject);

	TSharedRef<SWidget> CreateGridInventoriesContainer();
	TSharedRef<SWidget> CreatePropertiesWidget();

	UInventoryAsset* GetWorkingAsset() const;

private:
	void FillInventoriesOverlay();
	void SelectLastInventorySlot();

	FReply OnAddNewSlotClicked();
	FReply OnRemoveSelectedSlotClicked();
	FReply OnRemoveAllSlotsClicked();

	TWeakObjectPtr<UInventoryAsset> mWorkingAsset = nullptr;
	TWeakObjectPtr<UInventorySlotWrapper> mSelectedInventorySlot = nullptr;
	int32 mSelectedInventorySlotIndex = 0;
	TSharedPtr<SOverlay> mGridInventoriesRootOverlay = nullptr;
	TSharedPtr<IDetailsView> mSelectedInventorySlotDetailsView = nullptr;
};
