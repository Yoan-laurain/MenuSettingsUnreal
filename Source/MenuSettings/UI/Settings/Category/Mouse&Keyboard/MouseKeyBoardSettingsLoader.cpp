#include "PlayerMappableInputConfig.h"
#include "Configuration/BindingConfiguration.h"
#include "../GameSettingsCollection.h"
#include "../../../../Player/LocalPlayerCustom.h"
#include "../../LocalSettings.h"
#include "Configuration/MappableConfigPair.h"
#include "../SettingsManager.h"

#define LOCTEXT_NAMESPACE "MySettings"

UGameSettingsCollection* USettingsManager::InitializeMouseAndKeyboardSettings(const ULocalPlayerCustom* InLocalPlayer)
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(LOCTEXT("MouseAndKeyboardCollection_Name", "Mouse & Keyboard"));

	// Bindings for Mouse & Keyboard - Automatically Generated
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* Bindings = NewObject<UGameSettingsCollection>();
		Bindings->SetTitle(LOCTEXT("Bindings_Name", "Bindings"));
		
		Screen->AddSettingCollection(Bindings);

		//----------------------------------------------------------------------------------
		{ 
			static TSet<FName> AddedSettings;
			AddedSettings.Reset();

			//----------------------------------------------------------------------------------
			{
				const TArray<FLoadedMappableConfigPair>& RegisteredConfigs = InLocalPlayer->GetLocalSettings()->GetAllRegisteredInputConfigs();	
				const TMap<FName, FKey>& CustomKeyMap = InLocalPlayer->GetLocalSettings()->GetCustomPlayerInputConfig();

				// For all PMI ( Player Mappable Input ) configs
				for (const FLoadedMappableConfigPair& InputConfigPair : RegisteredConfigs)
				{
					// TODO : Handle gamepad
					if (InputConfigPair.Type != ECommonInputType::MouseAndKeyboard) 
					{
						continue;
					}
				
					TArray<FEnhancedActionKeyMapping> ConfigMappings = InputConfigPair.Config->GetPlayerMappableKeys();

					// For all IA ( Input Action ) configs
					for ( FEnhancedActionKeyMapping& Mapping : ConfigMappings )
					{
						UBindingConfiguration* ExistingSetting = nullptr;

						// Check if the player has already bound a key to this action
						for (UGameSettingsItem* Setting : Bindings->GetChildSettings()) 
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
								InputBinding->SetOptionName(Mapping.PlayerMappableOptions.DisplayName);
								InputBinding->SetWidgetType(ESettingsType::InputConfig);

								InputBinding->GetCurrentOptionValueDelegate().BindLambda([LocalSettings]() -> void
								{
									LocalSettings->ApplySettings(false);
								});
								
								InputBinding->ClearOptions();
								InputBinding->AddOption(InputBinding->GetPrimaryKeyText());
								
								Bindings->AddSetting(InputBinding);	
							}
					
							AddedSettings.Add(MappingSynthesized.PlayerMappableOptions.Name);
						}
					}
				}
			}
		}
	}

	return Screen;
}