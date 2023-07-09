#include "SettingsManager.h"
#include "GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"

USettingsManager::USettingsManager()
{

}

USettingsManager* USettingsManager::Get()
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GEngine->GetFirstGamePlayer(GWorld));

	if (LocalPlayer == nullptr)
	{
		return nullptr;
	}
	
	USettingsManager* Registry = FindObject<USettingsManager>(LocalPlayer, TEXT("SettingsManager"), true);
	 
	if (Registry == nullptr)
	{
		Registry = NewObject<USettingsManager>(LocalPlayer, TEXT("SettingsManager"));
	}

	return Registry;
}

void CallLambdas(UGameSettingsCollection* Setting)
{
	if ( Setting->GetChildSettingsCollection().Num() > 0 )
	{
		for (const auto& Option : Setting->GetChildSettingsCollection())
		{
			CallLambdas(Cast<UGameSettingsCollection>(Option));
		}
	}
	else
	{
		for ( const auto& Option : Setting->GetChildSettings() )
		{
			if ( !Option->GetCurrentOption().ToString().Equals("Custom") )
			{
				Option->ExecCurrentOptionValueDelegate();
			}
		}
	}
}

void USettingsManager::SaveChanges()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	
	for (const auto& Setting : SettingsMap)
	{
		CallLambdas(Setting.Value);
	}

	ensure(SettingsMap.Num() > 0);
	
	if ( LocalSettings )
	{
		LocalSettings->ApplySettings(false);
		LocalSettings->SaveSettings();
	}

}

void USettingsManager::CancelChanges()
{
	for (const auto& Setting : SettingsMap)
	{
		Setting.Value->CancelChanges();
	}
}

void USettingsManager::OnInitialize(ULocalPlayerCustom* InLocalPlayer)
{
	VideoSettings = InitializeVideoSettings();
	AudioSettings = InitializeAudioSettings();
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