#include "../GameSettingsCollection.h"
#include "../../../../Player/LocalPlayerCustom.h"
#include "../../LocalSettings.h"
#include "../SettingsManager.h"

#define LOCTEXT_NAMESPACE "MySettings"

UGameSettingsCollection* USettingsManager::InitializeBindingsSettings(const ULocalPlayerCustom* InLocalPlayer,ECommonInputType InputType)
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();

	if ( InputType == ECommonInputType::MouseAndKeyboard )
	{
		Screen->SetTitle(LOCTEXT("MouseAndKeyboardCollection_Name", "Mouse & Keyboard"));
		Screen->SetCategory(ESettingsCategory::MouseAndKeyboard);
	}
	else if (  InputType == ECommonInputType::Gamepad )
	{
		Screen->SetTitle(LOCTEXT("GamepadCollection_Name", "Gamepad"));
		Screen->SetCategory(ESettingsCategory::Gamepad);
	}
	
	// Bindings - Automatically Generated
	////////////////////////////////////////////////////////////////////////////////////
	// {
	// 	UGameSettingsCollection* Bindings = NewObject<UGameSettingsCollection>();
	// 	Bindings->SetTitle(LOCTEXT("Bindings_Name", "Bindings"));
	// 	
	// 	Screen->AddSettingCollection(Bindings);
	//
	// 	//----------------------------------------------------------------------------------
	// 	{ 
	// 		static TSet<FName> AddedSettings;
	// 		AddedSettings.Reset();
	//
	// 		//----------------------------------------------------------------------------------
	// 		{
	// 			const TArray<FLoadedMappableConfigPair>& RegisteredConfigs = InLocalPlayer->GetLocalSettings()->GetAllRegisteredInputConfigs();	
	// 			const TMap<FName, FKey>& CustomKeyMap = InLocalPlayer->GetLocalSettings()->GetCustomPlayerInputConfig();
	//
	// 			// For all PMI ( Player Mappable Input ) configs
	// 			for (const FLoadedMappableConfigPair& InputConfigPair : RegisteredConfigs)
	// 			{
	// 				if (InputConfigPair.Type != InputType) 
	// 				{
	// 					continue;
	// 				}
	// 			
	// 				TArray<FEnhancedActionKeyMapping> ConfigMappings = InputConfigPair.Config->GetPlayerMappableKeys();
	//
	// 				// For all IA ( Input Action ) configs
	// 				for ( FEnhancedActionKeyMapping& Mapping : ConfigMappings )
	// 				{
	// 					UBindingConfiguration* ExistingSetting = nullptr;
	//
	// 					// Check if the player has already bound a key to this action
	// 					for (UGameSettingsItem* Setting : Bindings->GetChildSettings()) 
	// 					{
	// 						UBindingConfiguration* KeyboardSetting = Cast<UBindingConfiguration>(Setting);
	//
	// 						if (KeyboardSetting->GetSettingDisplayName().EqualToCaseIgnored(Mapping.PlayerMappableOptions.DisplayName)) 
	// 						{
	// 							ExistingSetting = KeyboardSetting; 
	// 							break;
	// 						}
	// 					}
	// 			
	// 					FEnhancedActionKeyMapping MappingSynthesized(Mapping);
	// 					
	// 					// If the player has bound a custom key to this action, then set it to that
	// 					if (const FKey* PlayerBoundKey = CustomKeyMap.Find(Mapping.PlayerMappableOptions.Name))
	// 					{
	// 						MappingSynthesized.Key = *PlayerBoundKey;
	// 					}
	//
	// 					if (MappingSynthesized.PlayerMappableOptions.Name != NAME_None && !MappingSynthesized.PlayerMappableOptions.DisplayName.IsEmpty())
	// 					{
	// 						// Create the settings widget and initialize it, adding it to this config's section
	// 						UBindingConfiguration* InputBinding = ExistingSetting ? ExistingSetting : NewObject<UBindingConfiguration>();
	// 				
	// 						InputBinding->SetInputData(MappingSynthesized, InputConfigPair.Config, !ExistingSetting ? 0 : 1);
	// 				
	// 						if (!ExistingSetting)
	// 						{
	// 							InputBinding->SetOptionName(Mapping.PlayerMappableOptions.DisplayName);
	// 							InputBinding->SetWidgetType(ESettingsType::InputConfig);
	//
	// 							InputBinding->GetCurrentOptionValueDelegate().BindLambda([LocalSettings]() -> void
	// 							{
	// 								LocalSettings->ApplySettings(false);
	// 							});
	// 							
	// 							InputBinding->SetIsKeyboard(InputType == ECommonInputType::MouseAndKeyboard);
	// 							
	// 							InputBinding->ClearOptions();
	// 							InputBinding->AddOption(InputBinding->GetPrimaryKeyText());
	// 							
	// 							Bindings->AddSetting(InputBinding);	
	// 						}
	// 				
	// 						AddedSettings.Add(MappingSynthesized.PlayerMappableOptions.Name);
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	return Screen;
}