#include "CustomSettingKeyboardInput.h"
#include "EnhancedInputSubsystems.h"
#include "../GameSettingsCollection.h"
#include "../../../../Player/LocalPlayerCustom.h"
#include "../../LocalSettings.h"
#include "../SettingsManager.h"
#include "UserSettings/EnhancedInputUserSettings.h"

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
	{
		UGameSettingsCollection* KeyBinding = NewObject<UGameSettingsCollection>();
		KeyBinding->SetTitle(LOCTEXT("Bindings_Name", "Bindings"));
		
		Screen->AddSettingCollection(KeyBinding);

		//---------------------------------------------------------------------------------
		{
			static TSet<FName> AddedSettings;
			AddedSettings.Reset();
		}

			//----------------------------------------------------------------------------------
			{
				const UEnhancedInputLocalPlayerSubsystem* EISubsystem = InLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
				const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
			
			//----------------------------------------------------------------------------------
			
				static TSet<FName> CreatedMappingNames;
				CreatedMappingNames.Reset();
					
				for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
				{
					const FGameplayTag& ProfileName = ProfilePair.Key;
					const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;

					for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
					{
						// Create a setting row for anything with valid mappings and that we haven't created yet
						if (RowPair.Value.HasAnyMappings()  && !CreatedMappingNames.Contains(RowPair.Key))
						{
							// We only want keyboard keys on this settings screen, so we will filter down by mappings
							// that are set to keyboard keys
							FPlayerMappableKeyQueryOptions Options = {};
							Options.KeyToMatch = EKeys::W;
							Options.bMatchBasicKeyTypes = true;
							
							// Create the settings widget and initialize it, adding it to this config's section
							UCustomSettingKeyboardInput* InputBinding = NewObject<UCustomSettingKeyboardInput>();

							InputBinding->SetOptionName( RowPair.Value.Mappings.begin()->GetDisplayName() );
							InputBinding->SetWidgetType(ESettingsType::InputConfig);
							
							InputBinding->GetCurrentOptionValueDelegate().BindLambda([LocalSettings]() -> void
							{
								LocalSettings->ApplySettings(false);
							});

							InputBinding->SetIsKeyboard(InputType == ECommonInputType::MouseAndKeyboard);
							
							InputBinding->ClearOptions();
							
							FText KeyText = InputBinding->GetKeyTextFromSlot(EPlayerMappableKeySlot::First);
							
							InputBinding->AddOption(KeyText);
							
							InputBinding->InitializeInputData(Profile, RowPair.Value, Options);
							InputBinding->StoreInitial();
							
							KeyBinding->AddSetting(InputBinding);

							CreatedMappingNames.Add(RowPair.Key);
						}
					}
				}
			}
	}
	
	return Screen;
}