#include "BindingConfiguration.h"
#include "PlayerMappableInputConfig.h"
#include "../../../../../Player/LocalPlayerCustom.h"
#include "../../../LocalSettings.h"
#include "../../SettingsManager.h"

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
	FirstMappableOption.ResetToDefault();
	SecondaryMappableOption.ResetToDefault();

	if ( ULocalSettings* LocalSettings = USettingsManager::Get()->GetLocalPlayer()->GetLocalSettings() )
	{
		LocalSettings->AddOrUpdateCustomBindings(FirstMappableOption.InputMapping.PlayerMappableOptions.Name, FirstMappableOption.InputMapping.Key, USettingsManager::Get()->GetLocalPlayer(), isKeyboard);
	}
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
	
	ULocalPlayerCustom* LocalPlayer = USettingsManager::Get()->GetLocalPlayer();
	
	ULocalSettings* LocalSettings = LocalPlayer->GetLocalSettings();
	if (InKeyBindSlot == 0)
	{
		LocalSettings->AddOrUpdateCustomBindings(FirstMappableOption.InputMapping.PlayerMappableOptions.Name, NewKey, LocalPlayer, !NewKey.IsGamepadKey());
		FirstMappableOption.InputMapping.Key = NewKey;
	}
	// slot 2 removed for now
	else
	{
		ensureMsgf(false, TEXT("Invalid key bind slot provided!"));
	}

	return true;
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

	if (const ULocalPlayerCustom* LocalPlayer = USettingsManager::Get()->GetLocalPlayer())
	{
		ULocalSettings* LocalSettings = LocalPlayer->GetLocalSettings();
		LocalSettings->GetAllMappingNamesFromKey(Key, OutActionNames,isKeyboard);
	}
}

void UBindingConfiguration::ClearBindings()
{
	ChangeBinding(0, EKeys::Invalid);
	ChangeBinding(1, EKeys::Invalid);
}

void UBindingConfiguration::CancelChanges()
{
	RestoreToInitial();
}

bool UBindingConfiguration::ValueHasChanged()
{
	return  FirstMappableOption.InputMapping.Key != FirstMappableOption.GetInitialStoredValue();
}