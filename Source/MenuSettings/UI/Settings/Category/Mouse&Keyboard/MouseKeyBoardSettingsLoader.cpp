#include "PlayerMappableInputConfig.h"
#include "MenuSettings/UI/Settings/BindingsWTF/BindingConfiguration.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/Player/LocalPlayerCustom.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/BindingsWTF/MappableConfigPair.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

void AddPlayerMappableKey(TSet<FName>& AddedSettings, const TMap<FName, FKey>& CustomKeyMap, const FLoadedMappableConfigPair& InputConfigPair, TArray<FEnhancedActionKeyMapping> ConfigMappings, UGameSettingsCollection* ConfigSettingCollection)
{
	for (FEnhancedActionKeyMapping& Mapping : ConfigMappings)
	{
		UBindingConfiguration* ExistingSetting = nullptr;
						
		for (UGameSettingsItem* Setting : ConfigSettingCollection->GetChildSettings())
		{
			UBindingConfiguration* KeyboardSetting = Cast<UBindingConfiguration>(Setting);
			if (KeyboardSetting->GetSettingDisplayName().EqualToCaseIgnored(Mapping.PlayerMappableOptions.DisplayName))
			{
				ExistingSetting = KeyboardSetting; 
				break;
			}
		}
					
		FEnhancedActionKeyMapping MappingSynthesized(Mapping);
		// If the player has bound a custom key to this action, then set it to that
		if (const FKey* PlayerBoundKey = CustomKeyMap.Find(Mapping.PlayerMappableOptions.Name))
		{
			MappingSynthesized.Key = *PlayerBoundKey;
		}

		if (MappingSynthesized.PlayerMappableOptions.Name != NAME_None && !MappingSynthesized.PlayerMappableOptions.DisplayName.IsEmpty())
		{
			// Create the settings widget and initialize it, adding it to this config's section
			UBindingConfiguration* InputBinding = ExistingSetting ? ExistingSetting : NewObject<UBindingConfiguration>();
						
			InputBinding->SetInputData(MappingSynthesized, InputConfigPair.Config, !ExistingSetting ? 0 : 1);
						
			if (!ExistingSetting)
			{
				ConfigSettingCollection->AddSetting(InputBinding);	
			}
						
			AddedSettings.Add(MappingSynthesized.PlayerMappableOptions.Name);
		}
	}
}

UGameSettingsCollection* USettingsManager::InitializeMouseAndKeyboardSettings(const ULocalPlayerCustom* InLocalPlayer)
{
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Mouse & Keyboard"));

	// Bindings for Mouse & Keyboard - Automatically Generated
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* Volume = NewObject<UGameSettingsCollection>();
		Volume->SetTitle(FText::FromString("Bindings"));
		
		Screen->AddSettingCollection(Volume);

		//----------------------------------------------------------------------------------
		{ 
			static TSet<FName> AddedSettings;
			AddedSettings.Reset();

			//----------------------------------------------------------------------------------
			{
				const TArray<FLoadedMappableConfigPair>& RegisteredConfigs = InLocalPlayer->GetLocalSettings()->GetAllRegisteredInputConfigs();	
				const TMap<FName, FKey>& CustomKeyMap = InLocalPlayer->GetLocalSettings()->GetCustomPlayerInputConfig();
			
				for (const FLoadedMappableConfigPair& InputConfigPair : RegisteredConfigs)
				{
					if (InputConfigPair.Type != ECommonInputType::MouseAndKeyboard)
					{
						continue;
					}
				
					TArray<FEnhancedActionKeyMapping> ConfigMappings = InputConfigPair.Config->GetPlayerMappableKeys();
				
					UGameSettingsCollection* ConfigSettingCollection = NewObject<UGameSettingsCollection>();
					Screen->AddSetting(ConfigSettingCollection);
				
					AddPlayerMappableKey(AddedSettings, CustomKeyMap, InputConfigPair, ConfigMappings, ConfigSettingCollection);
				}
			}
		}
	}

	return Screen;
}