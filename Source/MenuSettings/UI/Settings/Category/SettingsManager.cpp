﻿#include "SettingsManager.h"
#include "GameSettingsCollection.h"
#include "Bindings/AssetManager/AssetManagerCustom.h"
#include "../Widget/Components/Basic/SettingsWidget.h"
#include "../LocalSettings.h"

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
		SetOptionToBase(Setting);
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
			if ( Option->ValueHasChangedCompareToStart() )
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
		CancelLocalSettings(Setting);
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
			if ( Option->ValueHasChangedCompareToDefault() )
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
		ResetLocalSettings(Setting);
	}

	ULocalSettings::Get()->ApplySettings(false);
}

UGameSettingsCollection* USettingsManager::GetSettings(const FString& SettingsName)
{
	for (const auto& Setting : SettingsMap)
	{
		if (Setting->GetTitle().ToString() == SettingsName)
		{
			return Setting;
		}
	}
	return nullptr;
}

void USettingsManager::OnInitialize(ULocalPlayerCustom* InLocalPlayer)
{
	VideoSettings = InitializeVideoSettings();
	AudioSettings = InitializeAudioSettings();
	MouseAndKeyboardSettings = InitializeBindingsSettings(InLocalPlayer,ECommonInputType::MouseAndKeyboard);
	GamepadSettings = InitializeBindingsSettings(InLocalPlayer,ECommonInputType::Gamepad);
	GameplaySettings = InitializeGameplaySettings();
	
	LocalPlayer = InLocalPlayer;

	SettingsMap.Add(GameplaySettings);
	SettingsMap.Add(MouseAndKeyboardSettings);
	SettingsMap.Add(VideoSettings);
	SettingsMap.Add(GamepadSettings);
	SettingsMap.Add(AudioSettings);
}

TArray<FString>* USettingsManager::InitializeNavigationsButtons() const
{
	TArray<FString>* NavigationButtons = new TArray<FString>();

	for (auto& Setting : SettingsMap)
	{
		NavigationButtons->Add(Setting->GetTitle().ToString());
	}
	return NavigationButtons;
}