#pragma once

#include "SettingsManager.generated.h"

struct FMappableConfigPair;

class UInputDataAsset;
class UGameSettingsCollection;
class ULocalPlayerCustom;

UCLASS()
class USettingsManager final : public UObject
{

	GENERATED_BODY()	
	
public:

	USettingsManager();

	static USettingsManager* Get();

	void SaveChanges();
	void CancelChanges(bool bWithBinding);
	void ResetToDefault();
	
	TArray<FString>* InitializeNavigationsButtons() const;
	
	UGameSettingsCollection* GetVideoSettings() const { return VideoSettings; }
	UGameSettingsCollection* GetAudioSettings() const { return AudioSettings; }
	UGameSettingsCollection* GetMouseAndKeyboardSettings() const { return MouseAndKeyboardSettings; }
	UGameSettingsCollection* GetGameplaySettings() const { return GameplaySettings; }
	
	UGameSettingsCollection* GetSettings(const FString SettingsName) const { return SettingsMap.FindRef(SettingsName); }
	void OnInitialize(ULocalPlayerCustom* InLocalPlayer);

	ULocalPlayerCustom* GetLocalPlayer() const { return LocalPlayer; }

protected :
	
	UGameSettingsCollection* InitializeVideoSettings();
	UGameSettingsCollection* InitializeAudioSettings();
	UGameSettingsCollection* InitializeMouseAndKeyboardSettings(const ULocalPlayerCustom* InLocalPlayer);
	UGameSettingsCollection* InitializeGameplaySettings();
	
	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> MouseAndKeyboardSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> GameplaySettings;

	UPROPERTY()
	TMap<FString, UGameSettingsCollection*> SettingsMap;

	UPROPERTY()
	ULocalPlayerCustom* LocalPlayer;
	
	static USettingsManager* Registry;
};