#include "../SettingsManager.h"
#include "MenuSettings/UI/Settings/GameSettingsCollection.h"

UGameSettingsCollection* USettingsManager::InitializeAudioSettings()
{
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
			OverallItem->SetNavigationText( FText::FromString("Overall Volume"));
			OverallItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of everything."));

			Volume->AddSetting(OverallItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* MusicItem = NewObject<UGameSettingsItem>();
			MusicItem->SetNavigationText( FText::FromString("Music Volume"));
			MusicItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of music."));

			Volume->AddSetting(MusicItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* EffectsItem = NewObject<UGameSettingsItem>();
			EffectsItem->SetNavigationText( FText::FromString("Effects Volume"));
			EffectsItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of sound effects."));

			Volume->AddSetting(EffectsItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* VoiceItem = NewObject<UGameSettingsItem>();
			VoiceItem->SetNavigationText( FText::FromString("Dialogue"));
			VoiceItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of dialogue for game characters and voice overs."));

			Volume->AddSetting(VoiceItem);
		}
	}

	return Screen;
}