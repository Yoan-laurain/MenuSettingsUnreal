#include "GameSettingsCollection.h"

UGameSettingsItem::UGameSettingsItem()
{
	SetBaseOptions();
}

void UGameSettingsItem::SetBaseOptions()
{
	const TArray<FString> OptionsToAdd = { "Low", "Medium", "High", "Epic" };
	
	SetOptions(OptionsToAdd);
}

UGameSettingsCollection::UGameSettingsCollection()
{
}

void UGameSettingsCollection::AddSetting(UGameSettingsItem* Setting)
{
	Settings.Add(Setting);
}