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
			Setting->SetDescriptionRichText(LOCTEXT("LanguageSetting_Description", "The language of the game."));
			
			Setting->GetCurrentOptionValueDelegate().BindLambda([LocalSettings,Setting]()
			{
				LocalSettings->SetPendingCulture(Setting->GetAvailableCultureNames()[Setting->GetIndexCurrentOption()]);
				LocalSettings->ApplyCultureSettings();
			});

			Setting->ClearOptions();
			
			Setting->InitializeCultureNames();
			
			for (const FString& CultureName : Setting->GetAvailableCultureNames())
			{
				if ( CultureName == TEXT(""))
				{
					continue;
				}
				Setting->AddOption(FText::FromString(CultureName));
			}
			
			int IndexCurrentCulture = Setting->GetAvailableCultureNames().Find(FInternationalization::Get().GetCurrentCulture()->GetName());

			// for editor
			if ( IndexCurrentCulture == -1)
				IndexCurrentCulture = 1;
			
			Setting->SetIndexCurrentOption(IndexCurrentCulture);
			Setting->SetDefaultOption(1);
			
			LanguageSubsection->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
	}

	return Screen;
}