#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"
#include "MenuSettings/UI/Settings/Scalar/GameSettingValueScalarDynamic.h"

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
			UGameSettingValueScalarDynamic* OverallItem = NewObject<UGameSettingValueScalarDynamic>();
			OverallItem->SetOptionName( FText::FromString("Overall Volume"));
			OverallItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of everything."));
			
			OverallItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetOverallVolume));
			OverallItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetOverallVolume));
			OverallItem->SetDefaultValue(GetDefault<ULocalSettings>()->GetOverallVolume());
			OverallItem->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			Volume->AddSetting(OverallItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueScalarDynamic* MusicItem = NewObject<UGameSettingValueScalarDynamic>();
			MusicItem->SetOptionName( FText::FromString("Music Volume"));
			MusicItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of music."));

			MusicItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetMusicVolume));
			MusicItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetMusicVolume));
			MusicItem->SetDefaultValue(GetDefault<ULocalSettings>()->GetMusicVolume());
			MusicItem->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			Volume->AddSetting(MusicItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueScalarDynamic* EffectsItem = NewObject<UGameSettingValueScalarDynamic>();
			EffectsItem->SetOptionName( FText::FromString("Effects Volume"));
			EffectsItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of sound effects."));

			EffectsItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetSoundFXVolume));
			EffectsItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetSoundFXVolume));
			EffectsItem->SetDefaultValue(GetDefault<ULocalSettings>()->GetSoundFXVolume());
			EffectsItem->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			Volume->AddSetting(EffectsItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueScalarDynamic* VoiceItem = NewObject<UGameSettingValueScalarDynamic>();
			VoiceItem->SetOptionName( FText::FromString("Dialogue"));
			VoiceItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of dialogue for game characters and voice overs."));

			VoiceItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetDialogueVolume));
			VoiceItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetDialogueVolume));
			VoiceItem->SetDefaultValue(GetDefault<ULocalSettings>()->GetDialogueVolume());
			VoiceItem->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);
			
			Volume->AddSetting(VoiceItem);
		}
	}

	return Screen;
}