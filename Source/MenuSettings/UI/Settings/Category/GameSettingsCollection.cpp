#include "GameSettingsCollection.h"
#include "../Widget/Components/Basic/SettingsWidget.h"

UGameSettingsItem::UGameSettingsItem()
{
	SetBaseOptions();
}

FIntPoint UGameSettingsItem::ConvertIntToFIntPoint(const int Value)
{
	switch (Value)
	{
		case 0:
			return FIntPoint(1280, 720);
		case 1:
			return FIntPoint(1920, 1080);
		case 2:
			return FIntPoint(2560, 1440);
		case 3:
			return FIntPoint(3840, 2160);
		default:
			return FIntPoint(1920, 1080);
	}
}

int UGameSettingsItem::ConvertFIntPointToInt(FIntPoint Value)
{
	if ( Value == FIntPoint(1280, 720) )
	{
		return 0;
	}
	if ( Value == FIntPoint(1920, 1080) )
	{
		return 1;
	}
	if ( Value == FIntPoint(2560, 1440) )
	{
		return 2;
	}
	if ( Value == FIntPoint(3840, 2160) )
	{
		return 3;
	}
	return 1;
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
	CurrentOptionValueDelegateSet.ExecuteIfBound();
}

void UGameSettingsItem::CancelChanges()
{
	IndexCurrentOption = BaseOption;
}

void UGameSettingsItem::AddDependentOption(UGameSettingsItem* DependentOption)
{
	DependentOptions.Add(DependentOption);
}

void UGameSettingsItem::ResetToDefault()
{
	IndexCurrentOption = DefaultOption;
}

int UGameSettingsItem::GetIndexFromFile() const
{
	if (MethodToGetIndexFromFile)
	{
		return MethodToGetIndexFromFile();
	}

	return 0;
}

void UGameSettingsItem::SetMethodToGetIndexFromFile(std::function<int()> Method)
{
	MethodToGetIndexFromFile = Method;
}

void UGameSettingsCollection::AddSetting(UGameSettingsItem* Setting)
{
	Settings.Add(Setting);
}

void UGameSettingsCollection::AddSettingCollection(UGameSettingsCollection* SettingCollection)
{
	SettingsCollection.Add(SettingCollection);
}