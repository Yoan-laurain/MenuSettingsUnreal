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
		//
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
		//
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
		//
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

bool UBindingConfiguration::ValueHasChangedCompareToStart()
{
	return  FirstMappableOption.InputMapping.Key != FirstMappableOption.GetInitialStoredValue();
}

bool UBindingConfiguration::ValueHasChangedCompareToDefault()
{
	return true;
}

void UBindingConfiguration::RegisterNewBinding(const FKey& InKey)
{
	ChangeBinding(0, InKey);
	
	ClearOptions();
	AddOption(FText::FromString(InKey.ToString()));
}