#include "GameSettingsCollection.h"
#include "../Widget/MenuSettingsWidget.h"
#include "../Widget/Components/UiSettingsParentClass.h"

#define LOCTEXT_NAMESPACE "MySettings"

UGameSettingsItem::UGameSettingsItem()
{
	SetBaseOptions();
}

bool UGameSettingsItem::ValueHasChangedCompareToStart()
{
	return IndexInitialOption != IndexCurrentOption;
}

bool UGameSettingsItem::ValueHasChangedCompareToDefault()
{
	return IndexDefaultOption != IndexCurrentOption;
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

int UGameSettingsItem::ConvertFIntPointToInt(const FIntPoint& Value)
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
	const TArray OptionsToAdd =
		{
			LOCTEXT("VisualEffectQualityLow", "Low"),
			LOCTEXT("VisualEffectQualityMedium", "Medium"),
			LOCTEXT("VisualEffectQualityHigh", "High"),
			LOCTEXT("VisualEffectQualityEpic", "Epic"),
		};
	
	SetOptions(OptionsToAdd);
}

UGameSettingsItem::FSetCurrentOptionValueDelegate& UGameSettingsItem::GetCurrentOptionValueDelegate()
{
	return CurrentOptionValueDelegateSet;
}

void UGameSettingsItem::ExecCurrentOptionValueDelegate()
{
	CurrentOptionValueDelegateSet.ExecuteIfBound();
	
	if ( GetWidget() && GetWidget()->GetParentWidget())
	{
		GetWidget()->GetParentWidget()->SetPendingModificationState(true);
	}
}

void UGameSettingsItem::CancelChanges()
{
	IndexCurrentOption = IndexInitialOption;
}

void UGameSettingsItem::AddDependentOption(UGameSettingsItem* DependentOption)
{
	DependentOptions.Add(DependentOption);
}

void UGameSettingsItem::ResetToDefault()
{
	IndexCurrentOption = IndexDefaultOption;
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