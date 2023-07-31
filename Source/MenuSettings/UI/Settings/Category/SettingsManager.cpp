#include "SettingsManager.h"
#include "GameSettingsCollection.h"
#include "Mouse&Keyboard/AssetManager/AssetManagerCustom.h"
#include "../Widget/Components/Basic/SettingsWidget.h"
#include "../InputDataAsset.h"
#include "../LocalSettings.h"
#include "Mouse&Keyboard/Configuration/BindingConfiguration.h"

#define LOCTEXT_NAMESPACE "MySettings"

USettingsManager* USettingsManager::Registry = nullptr;

USettingsManager* USettingsManager::Get()
{
	if ( !Registry )
	{
		Registry = NewObject<USettingsManager>();
		Registry->AddToRoot();
	}

	return Registry;
}

void SetOptionToBase(UGameSettingsCollection* Setting)
{
	if ( Setting->GetChildSettingsCollection().Num() > 0 )
	{
		for (const auto& Option : Setting->GetChildSettingsCollection())
		{
			SetOptionToBase(Cast<UGameSettingsCollection>(Option));
		}
	}
	else
	{
		for ( const auto& Option : Setting->GetChildSettings() )
		{
			Option->SetInitialIndex(Option->GetIndexCurrentOption());
		}
	}
} 

void USettingsManager::SaveChanges()
{
	for (const auto& Setting : SettingsMap)
	{
		SetOptionToBase(Setting.Value);
	}
}

void CancelLocalSettings( UGameSettingsCollection* Setting )
{
	if ( Setting->GetChildSettingsCollection().Num() > 0 )
	{
		for (const auto& Option : Setting->GetChildSettingsCollection())
		{
			CancelLocalSettings(Cast<UGameSettingsCollection>(Option));
		}
	}
	else
	{
		for ( const auto& Option : Setting->GetChildSettings() )
		{
			if ( Option->GetIndexCurrentOption() != Option->GetInitialIndex() || Option->IsA(UBindingConfiguration::StaticClass())  )
			{
				Option->CancelChanges();
				Option->ExecCurrentOptionValueDelegate();

				if ( Option->GetWidget() )
				{
					Option->GetWidget()->UpdateHUD();
				}
			}
		}
	}
}

void USettingsManager::CancelChanges()
{
	for (const auto& Setting : SettingsMap)
	{
		CancelLocalSettings(Setting.Value);
	}

	ULocalSettings::Get()->ApplySettings(false);
}

void ResetLocalSettings( UGameSettingsCollection* Setting )
{
	if ( Setting->GetChildSettingsCollection().Num() > 0 )
	{
		for (const auto& Option : Setting->GetChildSettingsCollection())
		{
			ResetLocalSettings(Cast<UGameSettingsCollection>(Option));
		}
	}
	else
	{
		for ( const auto& Option : Setting->GetChildSettings() )
		{
			if ( Option->GetIndexCurrentOption() != Option->GetDefaultOption() )
			{
				Option->ResetToDefault();
				Option->ExecCurrentOptionValueDelegate();

				if ( Option->GetWidget() )
				{
					Option->GetWidget()->UpdateHUD();
				}
			}
		}
	}
}

void USettingsManager::ResetToDefault()
{
	for (const auto& Setting : SettingsMap)
	{
		ResetLocalSettings(Setting.Value);
	}

	ULocalSettings::Get()->ApplySettings(false);
}

void USettingsManager::LoadAndRegisterInputConfigs()
{
	const UInputManager* InputManager = GetDefault<UInputManager>();
	InputManager->InputDataAsset.LoadSynchronous();

	// Check if the asset is valid after loading
	if (InputManager->InputDataAsset.IsValid())
	{
		UInputDataAsset* InputData = InputManager->InputDataAsset.Get();

		if (InputData)
		{
			for (const FMappableConfigPair& Pair : InputData->InputConfigs)
			{
				FMappableConfigPair::RegisterPair(Pair);
			}
		}
	}
}

void USettingsManager::OnInitialize(ULocalPlayerCustom* InLocalPlayer)
{
	LoadAndRegisterInputConfigs();
	
	VideoSettings = InitializeVideoSettings();
	AudioSettings = InitializeAudioSettings();
	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(InLocalPlayer);
	GameplaySettings = InitializeGameplaySettings();
	
	LocalPlayer = InLocalPlayer;
	
	TArray<FText> NavigationTexts;

	NavigationTexts.Add(LOCTEXT("Gameplay", "Gameplay"));
	NavigationTexts.Add(LOCTEXT("VideoCollection_Name", "Video"));
	NavigationTexts.Add(LOCTEXT("Audio", "Audio"));
	NavigationTexts.Add(LOCTEXT("MouseAndKeyboardCollection_Name", "Mouse & Keyboard"));

	SettingsMap.Add(NavigationTexts[0].ToString(), GameplaySettings);
	SettingsMap.Add(NavigationTexts[1].ToString(), VideoSettings);
	SettingsMap.Add(NavigationTexts[2].ToString(), AudioSettings);
	SettingsMap.Add(NavigationTexts[3].ToString(), MouseAndKeyboardSettings);
}

TArray<FString>* USettingsManager::InitializeNavigationsButtons() const
{
	TArray<FString>* NavigationButtons = new TArray<FString>();

	for (auto& Setting : SettingsMap)
	{
		NavigationButtons->Add(Setting.Key);
	}

	return NavigationButtons;
}