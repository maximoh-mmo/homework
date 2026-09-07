#include "GridInventoryNodes.h"

#define LOCTEXT_NAMESPACE "FGridInventoryNodesModule"

void FGridInventoryNodesModule::StartupModule()
{
}

void FGridInventoryNodesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGridInventoryNodesModule, GridInventoryNodes)