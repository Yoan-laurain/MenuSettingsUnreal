#include "AssetManagerCustom.h"

UAssetManagerCustom& UAssetManagerCustom::Get()
{
	check(GEngine);

	if (UAssetManagerCustom* Singleton = Cast<UAssetManagerCustom>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	
	return *NewObject<UAssetManagerCustom>();
}

UObject* UAssetManagerCustom::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{

		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UAssetManagerCustom::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}