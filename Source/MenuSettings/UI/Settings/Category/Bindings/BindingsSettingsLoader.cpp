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

				// A map of key bindings mapped to their display category
				TMap<FString, UGameSettingsCollection*> CategoryToSettingCollection;

				// Returns an existing setting collection for the display category if there is one.
				// If there isn't one, then it will create a new one and initialize it
				auto GetOrCreateSettingCollection = [&CategoryToSettingCollection, &Screen](FText DisplayCategory) -> UGameSettingsCollection*
				{
					FString DisplayCatString = DisplayCategory.ToString();
				
					if (UGameSettingsCollection** ExistingCategory = CategoryToSettingCollection.Find(DisplayCatString))
					{
						return *ExistingCategory;
					}
				
					UGameSettingsCollection* ConfigSettingCollection = NewObject<UGameSettingsCollection>();

					Screen->AddSetting(ConfigSettingCollection);
					CategoryToSettingCollection.Add(DisplayCatString, ConfigSettingCollection);
				
					return ConfigSettingCollection;
				};

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
																
							const FText& DesiredDisplayCategory = RowPair.Value.Mappings.begin()->GetDisplayCategory();
						
							if (UGameSettingsCollection* Collection = GetOrCreateSettingCollection(DesiredDisplayCategory))
							{
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
								
								FText KeyText = RowPair.Value.Mappings.begin()->GetCurrentKey().GetDisplayName();
								
								InputBinding->AddOption(KeyText);

								KeyBinding->AddSetting(InputBinding);
								Collection->AddSetting(InputBinding);

								CreatedMappingNames.Add(RowPair.Key);
							}
							else
							{
								ensure(false);
							}
						}
					}
				}
			}
	}
	
	return Screen;
}