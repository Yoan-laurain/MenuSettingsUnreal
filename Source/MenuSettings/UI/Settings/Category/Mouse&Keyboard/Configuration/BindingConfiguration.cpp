#include "BindingConfiguration.h"
#include "PlayerMappableInputConfig.h"
#include "MenuSettings/Player/LocalPlayerCustom.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

UBindingConfiguration::UBindingConfiguration()
{
}

void FKeyboardOption::ResetToDefault()
{
	if (OwningConfig)
	{
		InputMapping = OwningConfig->GetMappingByName(InputMapping.PlayerMappableOptions.Name);	
	}
	// If we don't have an owning config, then there is no default binding for this and it can simply be removed
	else
	{
		InputMapping = FEnhancedActionKeyMapping();
	}
}

void FKeyboardOption::SetInitialValue(FKey InKey)
{
	InitialMapping = InKey;
}

void UBindingConfiguration::SetInputData(FEnhancedActionKeyMapping& BaseMapping,
                                         const UPlayerMappableInputConfig* InOwningConfig, int32 InKeyBindSlot)
{
	if (InKeyBindSlot == 0)
	{
		FirstMappableOption.InputMapping = BaseMapping;
		FirstMappableOption.OwningConfig = InOwningConfig;
		FirstMappableOption.SetInitialValue(BaseMapping.Key);
	}
	else if (InKeyBindSlot == 1)
	{
		SecondaryMappableOption.InputMapping = BaseMapping;
		SecondaryMappableOption.OwningConfig = InOwningConfig;
		SecondaryMappableOption.SetInitialValue(BaseMapping.Key);
	}
	else
	{
		ensureMsgf(false, TEXT("Invalid key bind slot provided!"));
	}

	ensure(FirstMappableOption.InputMapping.PlayerMappableOptions.Name != NAME_None && !FirstMappableOption.InputMapping.PlayerMappableOptions.DisplayName.IsEmpty());

	const FString NameString = TEXT("KBM_Input_") + FirstMappableOption.InputMapping.PlayerMappableOptions.Name.ToString();
}

FText UBindingConfiguration::GetPrimaryKeyText() const
{
	return FirstMappableOption.InputMapping.Key.GetDisplayName();
}

FText UBindingConfiguration::GetSecondaryKeyText() const
{
	return SecondaryMappableOption.InputMapping.Key.GetDisplayName();
}

void UBindingConfiguration::StoreInitial()
{
	FirstMappableOption.SetInitialValue(FirstMappableOption.InputMapping.Key);
	SecondaryMappableOption.SetInitialValue(SecondaryMappableOption.InputMapping.Key);
}

void UBindingConfiguration::ResetToDefault()
{
	// Find the UPlayerMappableInputConfig that this came from and reset it to the value in there
	FirstMappableOption.ResetToDefault();
	SecondaryMappableOption.ResetToDefault();
}

void UBindingConfiguration::RestoreToInitial()
{
	ChangeBinding(0, FirstMappableOption.GetInitialStoredValue());
	ChangeBinding(1, SecondaryMappableOption.GetInitialStoredValue());
}

bool UBindingConfiguration::ChangeBinding(int32 InKeyBindSlot, FKey NewKey)
{
	// Early out if they hit the same button that is already bound. This allows for them to exit binding if they made a mistake.
	if ((InKeyBindSlot == 0 && FirstMappableOption.InputMapping.Key == NewKey) || (InKeyBindSlot == 1 && SecondaryMappableOption.InputMapping.Key == NewKey))
	{
		return false;
	}

	if (!NewKey.IsGamepadKey())
	{
		ULocalPlayerCustom* LocalPlayer = USettingsManager::Get()->GetLocalPlayer();
		
		ULocalSettings* LocalSettings = LocalPlayer->GetLocalSettings();
		if (InKeyBindSlot == 0)
		{
			LocalSettings->AddOrUpdateCustomKeyboardBindings(FirstMappableOption.InputMapping.PlayerMappableOptions.Name, NewKey, LocalPlayer);
			FirstMappableOption.InputMapping.Key = NewKey;
		}
		else if (InKeyBindSlot == 1)
		{
			// If there is no default secondary binding then we can create one based off of data from the primary binding
			if (SecondaryMappableOption.InputMapping.PlayerMappableOptions.Name == NAME_None)
			{
				SecondaryMappableOption = FKeyboardOption(FirstMappableOption);
			}
			
			LocalSettings->AddOrUpdateCustomKeyboardBindings(SecondaryMappableOption.InputMapping.PlayerMappableOptions.Name, NewKey, LocalPlayer);
			SecondaryMappableOption.InputMapping.Key = NewKey;
		}
		else
		{
			ensureMsgf(false, TEXT("Invalid key bind slot provided!"));
		}

		return true;
	}

	return false;
}

void UBindingConfiguration::GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key,
	TArray<FName>& OutActionNames) const
{
	if (InKeyBindSlot == 1)
	{
		if (SecondaryMappableOption.InputMapping.Key == Key)
		{
			return;
		}
	}
	else
	{
		if (FirstMappableOption.InputMapping.Key == Key)
		{
			return;
		}
	}

	if (const ULocalPlayerCustom* LyraLocalPlayer = USettingsManager::Get()->GetLocalPlayer())
	{
		ULocalSettings* LocalSettings = LyraLocalPlayer->GetLocalSettings();
		LocalSettings->GetAllMappingNamesFromKey(Key, OutActionNames);
	}
}

void UBindingConfiguration::Clear()
{
	ChangeBinding(0, EKeys::Invalid);
	ChangeBinding(1, EKeys::Invalid);
}