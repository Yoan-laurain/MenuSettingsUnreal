#pragma once
#include "CommonInputTypeEnum.h"
#include "Mouse&Keyboard/AssetManager/AssetManagerCustom.h"

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

	static USettingsManager* Get();

	void SaveChanges();
	void CancelChanges();
	void ResetToDefault();
	void LoadAndRegisterInputConfigs();

	TArray<FString>* InitializeNavigationsButtons() const;
	
	UGameSettingsCollection* GetVideoSettings() const { return VideoSettings; }
	UGameSettingsCollection* GetAudioSettings() const { return AudioSettings; }
	UGameSettingsCollection* GetMouseAndKeyboardSettings() const { return MouseAndKeyboardSettings; }
	UGameSettingsCollection* GetGameplaySettings() const { return GameplaySettings; }
	UGameSettingsCollection* GetGamepadSettings() const { return GamepadSettings; }
	
	UGameSettingsCollection* GetSettings(const FString SettingsName) const { return SettingsMap.FindRef(SettingsName); }
	void OnInitialize(ULocalPlayerCustom* InLocalPlayer);

	ULocalPlayerCustom* GetLocalPlayer() const { return LocalPlayer; }

	bool GetHasPendingModifications() const { return HasPendingModifications; }
	void SetHasPendingModifications(const bool bHasPendingModifications) { HasPendingModifications = bHasPendingModifications; }

protected :
	
	UGameSettingsCollection* InitializeVideoSettings();
	UGameSettingsCollection* InitializeAudioSettings();
	UGameSettingsCollection* InitializeBindingsSettings(const ULocalPlayerCustom* InLocalPlayer,ECommonInputType InputType);
	UGameSettingsCollection* InitializeGameplaySettings();
	
	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> MouseAndKeyboardSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> GamepadSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingsCollection> GameplaySettings;

	UPROPERTY()
	TMap<FString, UGameSettingsCollection*> SettingsMap;

	UPROPERTY()
	ULocalPlayerCustom* LocalPlayer;
	
	static USettingsManager* Registry;

	bool HasPendingModifications;
};