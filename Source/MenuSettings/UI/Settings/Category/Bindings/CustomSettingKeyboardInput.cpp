#include "CustomSettingKeyboardInput.h"
#include "EnhancedInputSubsystems.h"
#include "MenuSettings/Player/LocalPlayerCustom.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CustomSettingKeyboardInput)

class ULocalPlayer;

#define LOCTEXT_NAMESPACE "CustomSettings"

namespace Custom::ErrorMessages
{
	static const FText UnknownMappingName = LOCTEXT("CustomErrors_UnknownMappingName", "Unknown Mapping");
}


UCustomSettingKeyboardInput::UCustomSettingKeyboardInput()
{

}

FText UCustomSettingKeyboardInput::GetSettingDisplayName() const
{
	if (const FKeyMappingRow* Row = FindKeyMappingRow())
	{
		if (Row->HasAnyMappings())
		{
			return Row->Mappings.begin()->GetDisplayName();
		}
	}
	
	return Custom::ErrorMessages::UnknownMappingName;
}

FText UCustomSettingKeyboardInput::GetSettingDisplayCategory() const
{
	if (const FKeyMappingRow* Row = FindKeyMappingRow())
	{
		if (Row->HasAnyMappings())
		{
			return Row->Mappings.begin()->GetDisplayCategory();
		}
	}

	return Custom::ErrorMessages::UnknownMappingName;
}

const FKeyMappingRow* UCustomSettingKeyboardInput::FindKeyMappingRow() const
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		return Profile->FindKeyMappingRow(ActionMappingName);
	}

	ensure(false);
	return nullptr;
}

UEnhancedPlayerMappableKeyProfile* UCustomSettingKeyboardInput::FindMappableKeyProfile() const
{
	if (UEnhancedInputUserSettings* Settings = GetUserSettings())
	{
		return Settings->GetKeyProfileWithIdentifier(ProfileIdentifier);
	}
	
	ensure(false);
	return nullptr;
}

UEnhancedInputUserSettings* UCustomSettingKeyboardInput::GetUserSettings() const
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	if(ULocalPlayerCustom* CustomLocalPlayer = SettingsManager->GetLocalPlayer())
	{
		// Map the key to the player key profile
		if (UEnhancedInputLocalPlayerSubsystem* System = CustomLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			return System->GetUserSettings();
		}
	}
	
	return nullptr;
}

bool UCustomSettingKeyboardInput::ValueHasChangedCompareToStart()
{
	// return InitialKeyMappings[EPlayerMappableKeySlot::First] != GetKeyFromSlot(EPlayerMappableKeySlot::First) ||
	// 		InitialKeyMappings[EPlayerMappableKeySlot::Second] != GetKeyFromSlot(EPlayerMappableKeySlot::Second);
	return false;
}

bool UCustomSettingKeyboardInput::ValueHasChangedCompareToDefault()
{
	// TODO	:	HERE
	return false;
}

void UCustomSettingKeyboardInput::RegisterNewBinding(const FKey& InKey)
{
	ChangeBinding(0, InKey);
	
	ClearOptions();
	AddOption(FText::FromString(InKey.ToString()));
}

void UCustomSettingKeyboardInput::InitializeInputData(const UEnhancedPlayerMappableKeyProfile* KeyProfile, const FKeyMappingRow& MappingData, const FPlayerMappableKeyQueryOptions& InQueryOptions)
{
	check(KeyProfile);

	ProfileIdentifier = KeyProfile->GetProfileIdentifer();
	QueryOptions = InQueryOptions;
	
	for (const FPlayerKeyMapping& Mapping : MappingData.Mappings)
	{
		// Only add mappings that pass the query filters that have been provided upon creation
		if (!KeyProfile->DoesMappingPassQueryOptions(Mapping, QueryOptions))
		{
			continue;
		}
		
		ActionMappingName = Mapping.GetMappingName();
		InitialKeyMappings.Add(Mapping.GetSlot(), Mapping.GetCurrentKey());
	}
}

FText UCustomSettingKeyboardInput::GetKeyTextFromSlot(const EPlayerMappableKeySlot InSlot) const
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		FPlayerMappableKeyQueryOptions QueryOptionsForSlot = QueryOptions;
		QueryOptionsForSlot.SlotToMatch = InSlot;
		
		if (const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptionsForSlot))
				{
					return Mapping.GetCurrentKey().GetDisplayName();
				}
			}
		}
	}
	
	return EKeys::Invalid.GetDisplayName();
}

void UCustomSettingKeyboardInput::ResetToDefault()
{	
	if (UEnhancedInputUserSettings* Settings = GetUserSettings())
	{
		FMapPlayerKeyArgs Args = {};
		Args.MappingName = ActionMappingName;

		FGameplayTagContainer FailureReason;
		Settings->ResetAllPlayerKeysInRow(Args, FailureReason);
	}
}

void UCustomSettingKeyboardInput::StoreInitial()
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		if(const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptions))
				{
					ActionMappingName = Mapping.GetMappingName();
					InitialKeyMappings.Add(Mapping.GetSlot(), Mapping.GetCurrentKey());	
				}
			}
		}
	}
}

void UCustomSettingKeyboardInput::RestoreToInitial()
{
	for (TPair<EPlayerMappableKeySlot, FKey> Pair : InitialKeyMappings)
	{
		ChangeBinding((int32)Pair.Key, Pair.Value);	
	}
}

bool UCustomSettingKeyboardInput::ChangeBinding(int32 InKeyBindSlot, FKey NewKey)
{
	if (!NewKey.IsGamepadKey())
	{
		FMapPlayerKeyArgs Args = {};
		Args.MappingName = ActionMappingName;
		Args.Slot = (EPlayerMappableKeySlot) (static_cast<uint8>(InKeyBindSlot));
		Args.NewKey = NewKey;
		
		if (UEnhancedInputUserSettings* Settings = GetUserSettings())
		{
			FGameplayTagContainer FailureReason;
			Settings->MapPlayerKey(Args, FailureReason);
		}

		return true;
	}

	return false;
}

void UCustomSettingKeyboardInput::GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key, TArray<FName>& OutActionNames) const
{	
	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{		
		Profile->GetMappingNamesForKey(Key, OutActionNames);
	}
}

TArray<UCustomSettingKeyboardInput*> UCustomSettingKeyboardInput::GetAllMappedItemFromKey(TArray<FName>& OutActionNames)
{
	USettingsManager* SettingsManager = USettingsManager::Get();

	TArray<UCustomSettingKeyboardInput*> Items;

	UGameSettingsCollection* Collection = SettingsManager->GetMouseAndKeyboardSettings();
	
	for (UGameSettingsCollection* Category : Collection->GetChildSettingsCollection())
	{
		for ( UGameSettingsItem* Item : Category->GetChildSettings() )
		{
			if (UCustomSettingKeyboardInput* KeyboardInput = Cast<UCustomSettingKeyboardInput>(Item))
			{
				for (const FName& ActionName : OutActionNames)
				{
					if (KeyboardInput->ActionMappingName == ActionName)
					{
						Items.Add(KeyboardInput);
					}
				}
			}
		}
	}

	return Items;
}


bool UCustomSettingKeyboardInput::IsMappingCustomized() const
{
	bool bResult = false;

	if (const UEnhancedPlayerMappableKeyProfile* Profile = FindMappableKeyProfile())
	{
		FPlayerMappableKeyQueryOptions QueryOptionsForSlot = QueryOptions;

		if (const FKeyMappingRow* Row = FindKeyMappingRow())
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptionsForSlot))
				{
					bResult |= Mapping.IsCustomized();
				}
			}
		}
	}

	return bResult;
}

#undef LOCTEXT_NAMESPACE

