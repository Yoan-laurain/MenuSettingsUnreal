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

void USettingsManager::SaveChanges()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	
	for (const auto& Setting : SettingsMap)
	{
		Setting.Value->SetCurrentOptionValueDelegate();
	}

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