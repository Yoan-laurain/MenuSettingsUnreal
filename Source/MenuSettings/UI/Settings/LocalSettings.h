#pragma once

#include "GameFramework/GameUserSettings.h"
#include "Engine/DeveloperSettings.h"
#include "Category/Bindings/Configuration/MappableConfigPair.h"
#include "LocalSettings.generated.h"

DECLARE_EVENT_OneParam(ULocalSettings, FInputConfigDelegate, const FLoadedMappableConfigPair&);

class ULocalPlayerCustom;
class UPlayerMappableInputConfig;
class USoundControlBus;
class USoundControlBusMix;
class UBindingConfiguration;

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "AudioSettingsStaff"))
class MENUSETTINGS_API UAudioSettingsStaff final : public UDeveloperSettings
{
	GENERATED_BODY()

public :

#pragma region Buses
	
	UPROPERTY(config, EditAnywhere, Category = MixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBusMix"))
	FSoftObjectPath DefaultControlBusMix;
	
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBusMix"))
	FSoftObjectPath UserSettingsControlBusMix;
	
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath OverallVolumeControlBus;
	
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath MusicVolumeControlBus;
	
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath SoundFXVolumeControlBus;
	
	UPROPERTY(config, EditAnywhere, Category = UserMixSettings, meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath DialogueVolumeControlBus;

#pragma endregion Buses
};

UCLASS()
class ULocalSettings final : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	
	static ULocalSettings* Get();

#pragma region Audio
	
	UFUNCTION()
	void SetOverallVolume(float InVolume);
	
	UFUNCTION()
	void SetMusicVolume(float InVolume);
	
	UFUNCTION()
	void SetSoundFXVolume(float InVolume);
	
	UFUNCTION()
	void SetDialogueVolume(float InVolume);

	float GetOverallVolume() const;
	float GetMusicVolume() const;
	float GetSoundFXVolume() const;
	float GetDialogueVolume() const;

	void SetVolumeSettings( FName VolumeStringName, float Volume );

private :

	void LoadUserControlBusMix();

	void SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume);
	
	UPROPERTY(Transient)
	bool bSoundControlBusMixLoaded;

	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<USoundControlBus>> ControlBusMap;

	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> ControlBusMix = nullptr;

	UPROPERTY(Config)
	float OverallVolume = 1.0f;
	UPROPERTY(Config)
	float MusicVolume = 1.0f;
	UPROPERTY(Config)
	float SoundFXVolume = 1.0f;
	UPROPERTY(Config)
	float DialogueVolume = 1.0f;

#pragma endregion Audio

#pragma region KeyBindings
	
public :
	
	//////////////////////////////////////////////////////////////////
	// KeyBindings
	
	/** Delegate called when a new input config has been registered */
	FInputConfigDelegate OnInputConfigRegistered;

	/** Delegate called when a registered input config has been activated */
	FInputConfigDelegate OnInputConfigActivated;
	
	/** Delegate called when a registered input config has been deactivate */
	FInputConfigDelegate OnInputConfigDeactivated;
	
	/** Register the given input config with the settings to make it available to the player. */
	void RegisterInputConfig(ECommonInputType Type, const UPlayerMappableInputConfig* NewConfig);
	
	/** Unregister the given input config. Returns the number of configs removed. */
	int32 UnregisterInputConfig(const UPlayerMappableInputConfig* ConfigToRemove);

	/** Get all currently registered input configs */
	const TArray<FLoadedMappableConfigPair>& GetAllRegisteredInputConfigs() const { return RegisteredInputConfigs; }
	
	/**
	 * Returns the display name of any actions with that key bound to it
	 * 
	 * @param InKey The key to check for current mappings of
	 * @param OutActionNames Array to store display names of actions of bound keys
	 */
	void GetAllMappingNamesFromKey(const FKey InKey, TArray<FName>& OutActionNames, bool isKeyboard);

	/** Get an input config with a certain name. If the config doesn't exist then nullptr will be returned. */
	UFUNCTION(BlueprintCallable)
	const UPlayerMappableInputConfig* GetInputConfigByName(FName ConfigName) const;
	
	/**
	 * Maps the given keyboard setting to the new key
	 * 
	 * @param MappingName	The name of the FPlayerMappableKeyOptions that you would like to change
	 * @param NewKey		The new key to bind this option to
	 * @param LocalPlayer   local player to reset the keybinding on
	 */
	void AddOrUpdateCustomBindings(const FName MappingName, const FKey NewKey,ULocalPlayerCustom* LocalPlayer, bool isKeyboard);
	
	void GetAllBindingConfigurationsFromKey(int32 InKeyBindSlot,FKey Key,TArray<UBindingConfiguration*>& OutBindingConfiguration) const;
	
	/**
	 * Resets keybinding to its default value in its input mapping context 
	 * 
	 * @param MappingName	The name of the FPlayerMappableKeyOptions that you would like to change
	 * @param LocalPlayer   local player to reset the keybinding on
	 */
	void ResetKeybindingToDefault(const FName MappingName,ULocalPlayerCustom* LocalPlayer);

	/** Resets all keybindings to their default value in their input mapping context
	 * @param LocalPlayer   local player to reset the keybinding on
	 */
	void ResetKeybindingsToDefault(ULocalPlayerCustom* LocalPlayer);

	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }
	const TMap<FName, FKey>& GetCustomPlayerGamePadInputConfig() const { return CustomGamepadConfig; }

	/** The name of the current input config that the user has selected. */
	UPROPERTY(Config)
	FName InputConfigName = TEXT("Default");
	
	/**
	 * Array of currently registered input configs. This is populated by game feature plugins
	 * 
	 * @see UGameFeatureAction_AddInputConfig
	 */
	UPROPERTY(VisibleAnywhere)
	TArray<FLoadedMappableConfigPair> RegisteredInputConfigs;
	
	/** Array of custom key mappings that have been set by the player. Empty by default. */
	UPROPERTY(Config)
	TMap<FName, FKey> CustomKeyboardConfig;

	/** Array of custom key mappings that have been set by the player. Empty by default. */
	UPROPERTY(Config)
	TMap<FName, FKey> CustomGamepadConfig;

#pragma endregion KeyBindings

#pragma region GeneralSettings

public :

	/** Returns true if this user should run the auto benchmark as it has never been run */
	UFUNCTION(BlueprintCallable, Category = Settings)
	bool ShouldRunAutoBenchmarkAtStartup() const;

	/** Run the auto benchmark, optionally saving right away */
	UFUNCTION(BlueprintCallable, Category = Settings)
	void RunAutoBenchmark(bool bSaveImmediately);

	/** Apply just the quality scalability settings */
	void ApplyScalabilitySettings();

#pragma endregion GeneralSettings

#pragma region GameplaySettings

	////////////////////////////////////////////////////////
	// Culture / language
	public:
	/** Gets the pending culture */
	const FString& GetPendingCulture() const;

	/** Sets the pending culture to apply */
	void SetPendingCulture(const FString& NewCulture);

	// Called when the culture changes.
	void OnCultureChanged();

	/** Clears the pending culture to apply */
	void ClearPendingCulture();

	bool IsUsingDefaultCulture() const;

	void ResetToDefaultCulture();
	bool ShouldResetToDefaultCulture() const { return bResetToDefaultCulture; }
	
	void ApplyCultureSettings();
	virtual void ApplyNonResolutionSettings() override;
	void ResetCultureToCurrentSettings();

private:
	/** The pending culture to apply */
	UPROPERTY(Transient)
	FString PendingCulture;

	/* If true, resets the culture to default. */
	bool bResetToDefaultCulture = false;

	bool bIsDirty = false;

	////////////////////////////////////////////////////////
	
#pragma endregion GameplaySettings

public :
	
	UPROPERTY(Config)
	FName ControllerPlatform;
	
	ECommonInputType CurrentInputType;
};