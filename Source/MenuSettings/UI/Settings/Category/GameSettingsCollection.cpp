#include "GameSettingsCollection.h"

#include "MenuSettings/UI/Settings/Widget/Components/SettingsWidget.h"

UGameSettingsItem::UGameSettingsItem()
{
	SetBaseOptions();
}

void UGameSettingsItem::SetBaseOptions()
{
	const TArray OptionsToAdd = { FText::FromString("Low"), FText::FromString("Medium"), FText::FromString("High"), FText::FromString("Epic"), FText::FromString("Cinematic") };
	
	SetOptions(OptionsToAdd);
}

FText UGameSettingsItem::GetPreviousOptions(const FText& CurrentOption) 
{
	for ( int32 i = 0; i < Options.Num(); i++ )
	{
		if ( Options[i].EqualTo(CurrentOption) )
		{
			if ( i == 0 )
			{
				return Options[Options.Num() - 1];
			}
			return Options[i - 1];
		}
	}
	
	return Options[0];
}

FText UGameSettingsItem::GetNextOptions(const FText CurrentOption)
{
	for ( int32 i = 0; i < Options.Num(); i++ )
	{
		if ( Options[i].EqualTo(CurrentOption) )
		{
			if ( i == Options.Num() - 1 )
			{
				return Options[0];
			}
			return Options[i + 1];
		}
	}
	
	return Options[0];
}

UGameSettingsItem::FSetCurrentOptionValueDelegate& UGameSettingsItem::GetCurrentOptionValueDelegate()
{
	return CurrentOptionValueDelegateSet;
}

void UGameSettingsItem::ExecCurrentOptionValueDelegate()
{
	const bool Res = CurrentOptionValueDelegateSet.ExecuteIfBound();
	//ensure(Res);
}

void UGameSettingsItem::CancelChanges()
{
	//TODO - Implement this
}

void UGameSettingsItem::AddDependentOption(UGameSettingsItem* DependentOption)
{
	DependentOptions.Add(DependentOption);
}

UGameSettingsCollection::UGameSettingsCollection()
{
}

void UGameSettingsCollection::AddSetting(UGameSettingsItem* Setting)
{
	Settings.Add(Setting);
}

void UGameSettingsCollection::AddSettingCollection(UGameSettingsCollection* SettingCollection)
{
	SettingsCollection.Add(SettingCollection);
}