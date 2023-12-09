#include "CustomGameInstance.h"

#include "UI/Settings/Category/SettingsManager.h"

void UCustomGameInstance::Shutdown()
{
	Super::Shutdown();

	// Singleton isnt detroyed in the editor, so we need to clean it up manually
	USettingsManager::Get()->CleanUp();
}
