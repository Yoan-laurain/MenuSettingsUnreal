﻿#include "../../LocalSettings.h"
#include "SoundControlBus.h"
#include "AudioModulationStatics.h"

void ULocalSettings::SetOverallVolume(const float InVolume)
{
	OverallVolume = InVolume;
	SetVolumeSettings( TEXT("Overall"), InVolume );
}

void ULocalSettings::SetMusicVolume(const float InVolume)
{
	MusicVolume = InVolume;
	SetVolumeSettings( TEXT("Music"), InVolume );
}
void ULocalSettings::SetSoundFXVolume(const float InVolume)
{
	SetVolumeSettings(TEXT("SoundFX"), InVolume);
}

void ULocalSettings::SetDialogueVolume(const float InVolume)
{
	DialogueVolume = InVolume;
	SetVolumeSettings(TEXT("Dialogue"), InVolume);
}

float ULocalSettings::GetOverallVolume() const
{
	return OverallVolume;
}

float ULocalSettings::GetMusicVolume() const
{
	return MusicVolume;
}

float ULocalSettings::GetSoundFXVolume() const
{
	return SoundFXVolume;
}

float ULocalSettings::GetDialogueVolume() const
{
	return DialogueVolume;
}

void ULocalSettings::SetVolumeSettings(const FName VolumeStringName, const float Volume)
{
	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	// Ensure it's been loaded before continuing
	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));
	
	// Locate the locally cached bus and set the volume on it
	if (TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find( VolumeStringName ) )
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, Volume);
		}
	}
}

void ULocalSettings::LoadUserControlBusMix()
{
	if (GEngine)
	{
		if (const UWorld* World = GEngine->GetCurrentPlayWorld())
		{
			USoundControlBus* OverallControlBus = nullptr;
			USoundControlBus* MusicControlBus = nullptr;
			USoundControlBus* SoundFXControlBus = nullptr;
			USoundControlBus* DialogueControlBus = nullptr;

			ControlBusMap.Empty();

			if (UObject* ObjPath = OverallVolumeControlBus.TryLoad())
			{
				if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
				{
					OverallControlBus = SoundControlBus;
					ControlBusMap.Add(TEXT("Overall"), OverallControlBus);
				}
			}

			if (UObject* ObjPath = MusicVolumeControlBus.TryLoad())
			{
				if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
				{
					MusicControlBus = SoundControlBus;
					ControlBusMap.Add(TEXT("Music"), MusicControlBus);
				}
			}

			if (UObject* ObjPath = SoundFXVolumeControlBus.TryLoad())
			{
				if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
				{
					SoundFXControlBus = SoundControlBus;
					ControlBusMap.Add(TEXT("SoundFX"), SoundFXControlBus);
				}
			}

			if (UObject* ObjPath = DialogueVolumeControlBus.TryLoad())
			{
				if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
				{
					DialogueControlBus = SoundControlBus;
					ControlBusMap.Add(TEXT("Dialogue"), DialogueControlBus);
				}
			}
			
			if (UObject* ObjPath = UserSettingsControlBusMix.TryLoad())
			{
				if (USoundControlBusMix* SoundControlBusMix = Cast<USoundControlBusMix>(ObjPath))
				{
					ControlBusMix = SoundControlBusMix;

					const FSoundControlBusMixStage OverallControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, OverallControlBus, 1.f);
					const FSoundControlBusMixStage MusicControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, MusicControlBus, 1.f);
					const FSoundControlBusMixStage SoundFXControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, SoundFXControlBus, 1.f);
					const FSoundControlBusMixStage DialogueControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, DialogueControlBus, 1.f);

					TArray<FSoundControlBusMixStage> ControlBusMixStageArray;
					ControlBusMixStageArray.Add(OverallControlBusMixStage);
					ControlBusMixStageArray.Add(MusicControlBusMixStage);
					ControlBusMixStageArray.Add(SoundFXControlBusMixStage);
					ControlBusMixStageArray.Add(DialogueControlBusMixStage);

					UAudioModulationStatics::UpdateMix(World, ControlBusMix, ControlBusMixStageArray);

					bSoundControlBusMixLoaded = true;
				}
				else
				{
					ensureMsgf(SoundControlBusMix, TEXT("User Settings Control Bus Mix reference missing from Lyra Audio Settings."));
				}
			}
		}
	}
}

void ULocalSettings::SetVolumeForControlBus(USoundControlBus* InSoundControlBus, const float InVolume)
{
	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	// Ensure it's been loaded before continuing
	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));

	// Assuming everything has been loaded correctly, we retrieve the world and use AudioModulationStatics to update the Control Bus Volume values and
	// apply the settings to the cached User Control Bus Mix
	if (GEngine && InSoundControlBus && bSoundControlBusMixLoaded)
	{
		if (const UWorld* AudioWorld = GEngine->GetCurrentPlayWorld())
		{
			ensureMsgf(ControlBusMix, TEXT("Control Bus Mix failed to load."));

			// Create and set the Control Bus Mix Stage Parameters
			FSoundControlBusMixStage UpdatedControlBusMixStage;
			UpdatedControlBusMixStage.Bus = InSoundControlBus;
			UpdatedControlBusMixStage.Value.TargetValue = InVolume;
			UpdatedControlBusMixStage.Value.AttackTime = 0.01f;
			UpdatedControlBusMixStage.Value.ReleaseTime = 0.01f;

			// Add the Control Bus Mix Stage to an Array as the UpdateMix function requires
			TArray<FSoundControlBusMixStage> UpdatedMixStageArray;
			UpdatedMixStageArray.Add(UpdatedControlBusMixStage);

			// Modify the matching bus Mix Stage parameters on the User Control Bus Mix
			UAudioModulationStatics::UpdateMix(AudioWorld, ControlBusMix, UpdatedMixStageArray);
		}
	}
}