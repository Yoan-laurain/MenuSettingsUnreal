#include "MappableConfigPair.h"
#include "PlayerMappableInputConfig.h"
#include "AssetManagerCustom.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"

bool FMappableConfigPair::RegisterPair(const FMappableConfigPair& Pair)
{
	UAssetManagerCustom& AssetManager = UAssetManagerCustom::Get();

	if (ULocalSettings* Settings = ULocalSettings::Get())
	{
		// Register the pair with the settings, but do not activate it yet
		if (const UPlayerMappableInputConfig* LoadedConfig = AssetManager.GetAsset(Pair.Config))
		{
			Settings->RegisterInputConfig(Pair.Type, LoadedConfig, false);
			return true;
		}	
	}
	
	return false;
}

void FMappableConfigPair::UnregisterPair(const FMappableConfigPair& Pair)
{
	UAssetManagerCustom& AssetManager = UAssetManagerCustom::Get();
	
	if (ULocalSettings* Settings = ULocalSettings::Get())
	{
		if (const UPlayerMappableInputConfig* LoadedConfig = AssetManager.GetAsset(Pair.Config))
		{
			Settings->UnregisterInputConfig(LoadedConfig);
		}
	}
}