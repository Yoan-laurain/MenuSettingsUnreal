#include "Configuration/LanguageConfiguration.h"
#include "Internationalization/Culture.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

#define LOCTEXT_NAMESPACE "MySettings"

UGameSettingsCollection* USettingsManager::InitializeGameplaySettings()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Gameplay"));

	{
		UGameSettingsCollection* LanguageSubsection = NewObject<UGameSettingsCollection>();
		LanguageSubsection->SetTitle(LOCTEXT("LanguageCollection_Name", "Language"));
		
		Screen->AddSettingCollection(LanguageSubsection);

		//----------------------------------------------------------------------------------
		{
			ULanguageConfiguration* Setting = NewObject<ULanguageConfiguration>();
			Setting->SetOptionName( LOCTEXT("LanguageCollection_Name", "Language"));
			Setting->SetDescriptionRichText(LOCTEXT("LanguageSetting_Description", "The language of the game. You will need to restart the game completely for all language related changes to take effect."));

			Setting->InitializeCultureNames();
			Setting->ClearOptions();
			
			Setting->GetCurrentOptionValueDelegate().BindLambda([LocalSettings,Setting]()
			{
				LocalSettings->SetPendingCulture(Setting->GetAvailableCultureNames()[Setting->GetIndexCurrentOption()]);
				LocalSettings->ApplyCultureSettings();
			});
			
			for (const FString& CultureName : Setting->GetAvailableCultureNames())
			{
				Setting->AddOption(FText::FromString(CultureName));
			}
			
			Setting->SetDefaultOption(1);
			
			const FCultureRef CurrentCultureName = FInternationalization::Get().GetCurrentCulture();
			int IndexCurrentCulture = Setting->GetAvailableCultureNames().Find(CurrentCultureName->GetName());

			// for editor
			if ( IndexCurrentCulture == -1)
				IndexCurrentCulture = 1;
			
			Setting->SetIndexCurrentOption(IndexCurrentCulture);
			Setting->SetBaseOption(IndexCurrentCulture);
			
			LanguageSubsection->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
	}

	return Screen;
}