#pragma once

#include "SettingsManager.generated.h"

class UGameSettingsCollection;

UCLASS()
class USettingsManager final : public UObject
{

	GENERATED_BODY()	
	
public:

	USettingsManager();

	static USettingsManager* Get(ULocalPlayer* Player);

	void SaveChanges();
	TArray<FString>* InitializeNavigationsButtons() const;
	
	UGameSettingsCollection* GetVideoSettings() const { return VideoSettings; }
	UGameSettingsCollection* GetAudioSettings() const { return AudioSettings; }
	UGameSettingsCollection* GetSettings(const FString SettingsName) const { return SettingsMap.FindRef(SettingsName); }

protected :

	void OnInitialize(ULocalPlayer* Player);
	
	UGameSettingsCollection* InitializeVideoSettings(ULocalPlayer* Player);
	UGameSettingsCollection* InitializeAudioSettings();
	
	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> AudioSettings;
	
	TMap<FString, UGameSettingsCollection*> SettingsMap;
};