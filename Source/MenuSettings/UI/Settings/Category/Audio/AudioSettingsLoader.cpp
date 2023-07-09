#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

UGameSettingsCollection* USettingsManager::InitializeAudioSettings()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Audio"));
	
	TArray<int> TechnicalOption;
	for (int i = 0; i < 110; i+=10)
	{
		TechnicalOption.Add(i);
	}

	// Volume
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* Volume = NewObject<UGameSettingsCollection>();
		Volume->SetTitle(FText::FromString("Volume"));
		
		Screen->AddSettingCollection(Volume);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* OverallItem = NewObject<UGameSettingsItem>();
			OverallItem->SetOptionName( FText::FromString("Overall Volume"));
			OverallItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of everything."));
			
			OverallItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,OverallItem] ()
			{
				LocalSettings->SetOverallVolume(OverallItem->GetTechnicalOption() / 100.f);
			} );
			
			OverallItem->ClearOptions();
			for (int i = 0; i <= 100; i+=10)
			{
				OverallItem->AddOption(FText::FromString(FString::FromInt(i)));
			}
			
			OverallItem->SetTechnicalOption(TechnicalOption);
			OverallItem->SetIndexCurrentOption(LocalSettings->GetOverallVolume() * 10);

			Volume->AddSetting(OverallItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* MusicItem = NewObject<UGameSettingsItem>();
			MusicItem->SetOptionName( FText::FromString("Music Volume"));
			MusicItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of music."));
			
			MusicItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,MusicItem] ()
			{
				LocalSettings->SetMusicVolume(MusicItem->GetTechnicalOption()/100.f);
			} );
			
			MusicItem->ClearOptions();
			for (int i = 0; i <= 100; i+=10)
			{
				MusicItem->AddOption(FText::FromString(FString::FromInt(i)));
			}
			MusicItem->SetTechnicalOption(TechnicalOption);
			
			MusicItem->SetIndexCurrentOption(LocalSettings->GetMusicVolume() * 10);
			
			Volume->AddSetting(MusicItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* EffectsItem = NewObject<UGameSettingsItem>();
			EffectsItem->SetOptionName( FText::FromString("Effects Volume"));
			EffectsItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of sound effects."));
			
			EffectsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,EffectsItem] ()
			{
				LocalSettings->SetSoundFXVolume(EffectsItem->GetTechnicalOption()/100.f);
			} );
			
			EffectsItem->ClearOptions();
			for (int i = 0; i <= 100; i+=10)
			{
				EffectsItem->AddOption(FText::FromString(FString::FromInt(i)));
			}
			EffectsItem->SetTechnicalOption(TechnicalOption);
			EffectsItem->SetIndexCurrentOption(LocalSettings->GetSoundFXVolume() * 10);
			
			Volume->AddSetting(EffectsItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* VoiceItem = NewObject<UGameSettingsItem>();
			VoiceItem->SetOptionName( FText::FromString("Dialogue"));
			VoiceItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of dialogue for game characters and voice overs."));
			
			VoiceItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,VoiceItem] ()
			{
				LocalSettings->SetDialogueVolume(VoiceItem->GetTechnicalOption()/100.f);
			} );
			
			VoiceItem->ClearOptions();
			for (int i = 0; i <= 100; i+=10)
			{
				VoiceItem->AddOption(FText::FromString(FString::FromInt(i)));
			}
			VoiceItem->SetTechnicalOption(TechnicalOption);
			VoiceItem->SetIndexCurrentOption(LocalSettings->GetDialogueVolume() * 10);
			
			Volume->AddSetting(VoiceItem);
		}
	}

	return Screen;
}