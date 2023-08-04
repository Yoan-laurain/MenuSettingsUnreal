#include "../../LocalSettings.h"
#include "../GameSettingsCollection.h"
#include "../SettingsManager.h"

#define LOCTEXT_NAMESPACE "MySettings"

UGameSettingsCollection* USettingsManager::InitializeAudioSettings()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Audio"));
	Screen->SetCategory(ESettingsCategory::AudioSystem);
	
	TArray<int> TechnicalOption;
	for (int i = 0; i <= 100; i++)
	{
		TechnicalOption.Add(i);
	}

	TArray<FText> OptionsText;
	for (int i = 0; i <= 100; i++)
	{
		OptionsText.Add(FText::FromString(FString::FromInt(i)));
	}

	const int DefaultValue = 100;

	// Volume
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* Volume = NewObject<UGameSettingsCollection>();
		Volume->SetTitle(LOCTEXT("VolumeCollection_Name", "Volume"));
		
		Screen->AddSettingCollection(Volume);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* OverallItem = NewObject<UGameSettingsItem>();
			OverallItem->SetOptionName( LOCTEXT("OverallVolume_Name", "Overall Volume"));
			OverallItem->SetDescriptionRichText(LOCTEXT("OverallVolume_Description", "Adjusts the volume of everything."));
			OverallItem->SetWidgetType(ESettingsType::Slider);
			
			OverallItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,OverallItem] ()
			{
				LocalSettings->SetOverallVolume(OverallItem->GetTechnicalOption() / 100.f);
			} );
			
			OverallItem->SetOptions(OptionsText);
			
			OverallItem->SetTechnicalOption(TechnicalOption);
			OverallItem->SetIndexCurrentOption(LocalSettings->GetOverallVolume() * 100);
			OverallItem->SetDefaultOption(DefaultValue);

			Volume->AddSetting(OverallItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* MusicItem = NewObject<UGameSettingsItem>();
			MusicItem->SetOptionName( LOCTEXT("MusicVolume_Name", "Music"));
			MusicItem->SetDescriptionRichText(LOCTEXT("MusicVolume_Description", "Adjusts the volume of music."));
			MusicItem->SetWidgetType(ESettingsType::Slider);
			
			MusicItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,MusicItem] ()
			{
				LocalSettings->SetMusicVolume(MusicItem->GetTechnicalOption()/100.f);
			} );
			
			MusicItem->SetOptions(OptionsText);

			MusicItem->SetTechnicalOption(TechnicalOption);
			
			MusicItem->SetIndexCurrentOption(LocalSettings->GetMusicVolume() * 100);
			MusicItem->SetDefaultOption(DefaultValue);
			
			Volume->AddSetting(MusicItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* EffectsItem = NewObject<UGameSettingsItem>();
			EffectsItem->SetOptionName( LOCTEXT("SoundEffectsVolume_Name", "Sound Effects"));
			EffectsItem->SetDescriptionRichText(LOCTEXT("SoundEffectsVolume_Description", "Adjusts the volume of sound effects."));
			EffectsItem->SetWidgetType(ESettingsType::Slider);
			
			EffectsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,EffectsItem] ()
			{
				LocalSettings->SetSoundFXVolume(EffectsItem->GetTechnicalOption()/100.f);
			} );
			
			EffectsItem->SetOptions(OptionsText);

			EffectsItem->SetTechnicalOption(TechnicalOption);
			EffectsItem->SetIndexCurrentOption(LocalSettings->GetSoundFXVolume() * 100);
			EffectsItem->SetDefaultOption(DefaultValue);
			
			Volume->AddSetting(EffectsItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* VoiceItem = NewObject<UGameSettingsItem>();
			VoiceItem->SetOptionName( LOCTEXT("DialogueVolume_Name", "Dialogue"));
			VoiceItem->SetDescriptionRichText(LOCTEXT("DialogueVolume_Description", "Adjusts the volume of dialogue for game characters and voice overs."));
			VoiceItem->SetWidgetType(ESettingsType::Slider);
			
			VoiceItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,VoiceItem] ()
			{
				LocalSettings->SetDialogueVolume(VoiceItem->GetTechnicalOption()/100.f);
			} );
			
			VoiceItem->SetOptions(OptionsText);
			
			VoiceItem->SetTechnicalOption(TechnicalOption);
			VoiceItem->SetIndexCurrentOption(LocalSettings->GetDialogueVolume() * 100);
			VoiceItem->SetDefaultOption(DefaultValue);
			
			Volume->AddSetting(VoiceItem);
		}
	}

	return Screen;
}