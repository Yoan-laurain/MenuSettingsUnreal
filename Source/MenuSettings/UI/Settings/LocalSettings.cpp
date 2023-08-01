#include "LocalSettings.h"

#include "CommonInputSubsystem.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputSubsystems.h"
#include "../../Player/LocalPlayerCustom.h"
#include "PlayerMappableInputConfig.h"
#include "Category/GameSettingsCollection.h"
#include "Category/SettingsManager.h"
#include "Category/Mouse&Keyboard/Configuration/BindingConfiguration.h"
#include "Internationalization/Culture.h"

ULocalSettings* ULocalSettings::Get()
{
	return GEngine ? CastChecked<ULocalSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void ULocalSettings::RegisterInputConfig(ECommonInputType Type, const UPlayerMappableInputConfig* NewConfig)
{
	if (NewConfig)
	{
		const int32 ExistingConfigIdx = RegisteredInputConfigs.IndexOfByPredicate( [&NewConfig](const FLoadedMappableConfigPair& Pair) { return Pair.Config == NewConfig; } );
		if (ExistingConfigIdx == INDEX_NONE)
		{
			const int32 NumAdded = RegisteredInputConfigs.Add(FLoadedMappableConfigPair(NewConfig, Type));
			if (NumAdded != INDEX_NONE)
			{
				OnInputConfigRegistered.Broadcast(RegisteredInputConfigs[NumAdded]);
			}	
		}
	}
}

int32 ULocalSettings::UnregisterInputConfig(const UPlayerMappableInputConfig* ConfigToRemove)
{
	if (ConfigToRemove)
	{
		const int32 Index = RegisteredInputConfigs.IndexOfByPredicate( [&ConfigToRemove](const FLoadedMappableConfigPair& Pair) { return Pair.Config == ConfigToRemove; } );
		if (Index != INDEX_NONE)
		{
			RegisteredInputConfigs.RemoveAt(Index);
			return 1;
		}
			
	}
	return INDEX_NONE;
}

void ULocalSettings::GetAllMappingNamesFromKey(const FKey InKey, TArray<FName>& OutActionNames)
{
	if (InKey == EKeys::Invalid)
	{
		return;
	}

	// adding any names of actions that are bound to that key
	for (const FLoadedMappableConfigPair& Pair : RegisteredInputConfigs)
	{
		if (Pair.Type == ECommonInputType::MouseAndKeyboard)
		{
			for (const FEnhancedActionKeyMapping& Mapping : Pair.Config->GetPlayerMappableKeys())
			{
				FName MappingName(Mapping.PlayerMappableOptions.DisplayName.ToString());
				FName ActionName = Mapping.PlayerMappableOptions.Name;
				// make sure it isn't custom bound as well
				if (const FKey* MappingKey = CustomKeyboardConfig.Find(ActionName))
				{
					if (*MappingKey == InKey)
					{
						OutActionNames.Add(MappingName);
					}
				}
				else
				{
					if (Mapping.Key == InKey)
					{
						OutActionNames.Add(MappingName);
					}
				}
			}
		}
	}
}

const UPlayerMappableInputConfig* ULocalSettings::GetInputConfigByName(FName ConfigName) const
{
	for (const FLoadedMappableConfigPair& Pair : RegisteredInputConfigs)
	{
		if (Pair.Config->GetConfigName() == ConfigName)
		{
			return Pair.Config;
		}
	}
	return nullptr;
}

void ULocalSettings::AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey,ULocalPlayerCustom* LocalPlayer)
{
	if (MappingName == NAME_None)
	{
		return;
	}
	
	if (InputConfigName != TEXT("Custom"))
	{
		// Copy Presets.
		if (const UPlayerMappableInputConfig* DefaultConfig = GetInputConfigByName(TEXT("Default")))
		{
			for (const FEnhancedActionKeyMapping& Mapping : DefaultConfig->GetPlayerMappableKeys())
			{
				// Make sure that the mapping has a valid name, its possible to have an empty name
				// if someone has marked a mapping as "Player Mappable" but deleted the default field value
				if (Mapping.PlayerMappableOptions.Name != NAME_None)
				{
					CustomKeyboardConfig.Add(Mapping.PlayerMappableOptions.Name, Mapping.Key);
				}
			}
		}
		
		InputConfigName = TEXT("Custom");
	} 

	if (FKey* ExistingMapping = CustomKeyboardConfig.Find(MappingName))
	{
		// Change the key to the new one
		CustomKeyboardConfig[MappingName] = NewKey;
	}
	else
	{
		CustomKeyboardConfig.Add(MappingName, NewKey);
	}

	// Tell the enhanced input subsystem for this local player that we should remap some input! Woo
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->AddPlayerMappedKeyInSlot(MappingName, NewKey);
	}
}

void ULocalSettings::GetAllBindingConfigurationsFromKey(int32 InKeyBindSlot, FKey Key,
	TArray<UBindingConfiguration*>& OutBindingConfiguration) const
{
	const USettingsManager* SettingsManager = USettingsManager::Get();

	if ( SettingsManager )
	{
		UGameSettingsCollection* MouseAndKeyboardSettings = SettingsManager->GetMouseAndKeyboardSettings();

		for ( const auto& SettingCol : MouseAndKeyboardSettings->GetChildSettingsCollection() )
		{
			if ( SettingCol )
			{
				for ( const auto& Setting : SettingCol->GetChildSettings() )
				{
					if ( Setting )
					{
						if ( UBindingConfiguration* BindingConfig = Cast<UBindingConfiguration>(Setting) )
						{
							if ( InKeyBindSlot == 0 )
							{
								if ( BindingConfig->GetFirstMappableOption().InputMapping.Key == Key )
								{
									OutBindingConfiguration.Add(BindingConfig);
								}
							}
							else if ( InKeyBindSlot == 1 )
							{
								if ( BindingConfig->GetSecondaryMappableOption().InputMapping.Key == Key )
								{
									OutBindingConfiguration.Add(BindingConfig);
								}
							}
						}
					}
				}
			}
		}
	}
}

void ULocalSettings::ResetKeybindingToDefault(const FName MappingName,ULocalPlayerCustom* LocalPlayer)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->RemovePlayerMappedKeyInSlot(MappingName); 
	}
}

void ULocalSettings::ResetKeybindingsToDefault(ULocalPlayerCustom* LocalPlayer)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->RemoveAllPlayerMappedKeys();
	}
}

//////////////////////////////////////////////////////////////////////

void ULocalSettings::ApplyCultureSettings()
{
	if (bResetToDefaultCulture)
	{
		const FCulturePtr SystemDefaultCulture = FInternationalization::Get().GetDefaultCulture();
		check(SystemDefaultCulture.IsValid());

		const FString CultureToApply = SystemDefaultCulture->GetName();
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			// Clear this string
			GConfig->RemoveKey(TEXT("Internationalization"), TEXT("Culture"), GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		bResetToDefaultCulture = false;
	}
	else if (!PendingCulture.IsEmpty())
	{
		// SetCurrentCulture may trigger PendingCulture to be cleared (if a culture change is broadcast) so we take a copy of it to work with
		const FString CultureToApply = PendingCulture;
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			// Note: This is intentionally saved to the users config
			// We need to localize text before the player logs
			GConfig->SetString(TEXT("Internationalization"), TEXT("Culture"), *CultureToApply, GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		ClearPendingCulture();
	}
}

void ULocalSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();
}

void ULocalSettings::ResetCultureToCurrentSettings()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

const FString& ULocalSettings::GetPendingCulture() const
{
	return PendingCulture;
}

void ULocalSettings::SetPendingCulture(const FString& NewCulture)
{
	PendingCulture = NewCulture;
	bResetToDefaultCulture = false;
	bIsDirty = true;
}

void ULocalSettings::OnCultureChanged()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

void ULocalSettings::ClearPendingCulture()
{
	PendingCulture.Reset();
}

bool ULocalSettings::IsUsingDefaultCulture() const
{
	FString Culture;
	GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni);

	return Culture.IsEmpty();
}

void ULocalSettings::ResetToDefaultCulture()
{
	ClearPendingCulture();
	bResetToDefaultCulture = true;
	bIsDirty = true;
}

//////////////////////////////////////////////////////////////////////