#include "LanguageConfiguration.h"

#include "../../../LocalSettings.h"

static constexpr int32 SettingSystemDefaultLanguageIndex = 0;

void ULanguageConfiguration::InitializeCultureNames()
{
	TArray<FString> AllCultureNames = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
	for (FString& CultureName : AllCultureNames)
	{
		if ( CultureName == TEXT(""))
		{
			continue;
		}
		
		if (FInternationalization::Get().IsCultureAllowed(CultureName))
		{
			AvailableCultureNames.Add(CultureName);
		}
	}

	//AvailableCultureNames.Insert(TEXT(""), SettingSystemDefaultLanguageIndex);
}

void ULanguageConfiguration::ResetToDefault()
{
	if (ULocalSettings* Settings = ULocalSettings::Get() ) 
	{
		Settings->ResetToDefaultCulture();
	}
}

void ULanguageConfiguration::RestoreToInitial()
{
	if (ULocalSettings* Settings = ULocalSettings::Get())
	{
		Settings->ClearPendingCulture();
	}
}

TArray<FString> ULanguageConfiguration::GetAvailableCultureNames()
{
	return AvailableCultureNames;
}