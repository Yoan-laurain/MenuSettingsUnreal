﻿#include "SettingsManager.h"
#include "GameSettingsCollection.h"
#include "Engine/AssetManager.h"
#include "Mouse&Keyboard/AssetManager/AssetManagerCustom.h"
#include "../Widget/Components/Basic/SettingsWidget.h"
#include "../InputDataAsset.h"
#include "../LocalSettings.h"
#include "Mouse&Keyboard/Configuration/BindingConfiguration.h"

USettingsManager* USettingsManager::Registry = nullptr;

USettingsManager::USettingsManager()
{

}

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
			Option->SetBaseOption(Option->GetIndexCurrentOption());
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
			if ( Option->GetIndexCurrentOption() != Option->GetBaseOption() || Option->IsA(UBindingConfiguration::StaticClass())  )
			{
				Option->CancelChanges();
				Option->ExecCurrentOptionValueDelegate();
				Option->GetWidget()->UpdateHUD();
			}
		}
	}
}

void USettingsManager::CancelChanges(bool bWithBinding)
{
	for (const auto& Setting : SettingsMap)
	{
		if ( !bWithBinding && Setting.Key == "Mouse and Keyboard" )
		{
			continue;
		}
		CancelLocalSettings(Setting.Value);
	}

	ULocalSettings::Get()->ApplySettings(false);
}

void USettingsManager::OnInitialize(ULocalPlayerCustom* InLocalPlayer)
{
	VideoSettings = InitializeVideoSettings();
	AudioSettings = InitializeAudioSettings();

	const UInputManager* InputManager = GetDefault<UInputManager>();

	UE_LOG( LogTemp, Warning, TEXT( "InputManager->InputDataAsset.IsValid() = %d" ), InputManager->InputDataAsset.IsValid() );
	
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
	
	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(InLocalPlayer);
	LocalPlayer = InLocalPlayer;

	SettingsMap.Add("Video", VideoSettings);
	SettingsMap.Add("Audio", AudioSettings);
	SettingsMap.Add("Mouse and Keyboard", MouseAndKeyboardSettings);
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