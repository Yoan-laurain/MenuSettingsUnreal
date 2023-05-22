#pragma once

#include "LocalPlayerCustom.h"
#include "Scalar/GameSettingDataSourceDynamic.h"
#include "SettingsManager.generated.h"

class UGameSettingsCollection;

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(ULocalPlayerCustom, GetLocalSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(ULocalSettings, FunctionOrPropertyName)		    \
		}))

UCLASS()
class USettingsManager final : public UObject
{

	GENERATED_BODY()	
	
public:

	USettingsManager();

	static USettingsManager* Get();

	void SaveChanges();
	TArray<FString>* InitializeNavigationsButtons() const;
	
	UGameSettingsCollection* GetVideoSettings() const { return VideoSettings; }
	UGameSettingsCollection* GetAudioSettings() const { return AudioSettings; }
	UGameSettingsCollection* GetSettings(const FString SettingsName) const { return SettingsMap.FindRef(SettingsName); }
	void OnInitialize(ULocalPlayerCustom* InLocalPlayer);

	ULocalPlayerCustom* GetLocalPlayer() const { return LocalPlayer; }
	
protected :
	
	UGameSettingsCollection* InitializeVideoSettings();
	UGameSettingsCollection* InitializeAudioSettings();
	UGameSettingsCollection* InitializeMouseAndKeyboardSettings(const ULocalPlayerCustom* InLocalPlayer);
	
	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> MouseAndKeyboardSettings;
	
	TMap<FString, UGameSettingsCollection*> SettingsMap;

	UPROPERTY()
	ULocalPlayerCustom* LocalPlayer;
	
};