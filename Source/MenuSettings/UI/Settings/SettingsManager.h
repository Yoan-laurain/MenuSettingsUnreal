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

	UGameSettingsCollection* GetVideoSettings() const { return VideoSettings; }

protected :

	void OnInitialize(ULocalPlayer* Player);
	
	UGameSettingsCollection* InitializeVideoSettings(ULocalPlayer* Player);
	// UGameSettingsCollection* InitializeAudioSettings();
	
	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> VideoSettings;

	// UPROPERTY()
	// TObjectPtr<UGameSettingCollection> AudioSettings;
};