#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

UGameSettingsCollection* USettingsManager::InitializeAudioSettings()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Audio"));

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
			
			OverallItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,OverallItem] ()
			{
				LocalSettings->SetOverallVolume(OverallItem->GetCurrentValue<float>());
			} );
			
			OverallItem->SetDefaultValue(1.f);

			Volume->AddSetting(OverallItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* MusicItem = NewObject<UGameSettingsItem>();
			MusicItem->SetOptionName( FText::FromString("Music Volume"));
			MusicItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of music."));
			
			MusicItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,MusicItem] ()
			{
				LocalSettings->SetMusicVolume(MusicItem->GetCurrentValue<float>());
			} );
			
			MusicItem->SetDefaultValue(1.f);
			
			Volume->AddSetting(MusicItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* EffectsItem = NewObject<UGameSettingsItem>();
			EffectsItem->SetOptionName( FText::FromString("Effects Volume"));
			EffectsItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of sound effects."));
			
			EffectsItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,EffectsItem] ()
			{
				LocalSettings->SetSoundFXVolume(EffectsItem->GetCurrentValue<float>());
			} );
			
			EffectsItem->SetDefaultValue(1.f);

			Volume->AddSetting(EffectsItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* VoiceItem = NewObject<UGameSettingsItem>();
			VoiceItem->SetOptionName( FText::FromString("Dialogue"));
			VoiceItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of dialogue for game characters and voice overs."));
			
			VoiceItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,VoiceItem] ()
			{
				LocalSettings->SetDialogueVolume(VoiceItem->GetCurrentValue<float>());
			} );
			
			VoiceItem->SetDefaultValue(1.f);
			
			Volume->AddSetting(VoiceItem);
		}
	}

	return Screen;
}