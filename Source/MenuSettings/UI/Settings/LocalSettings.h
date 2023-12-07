#pragma once

#include "InputMappingContext.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/DeveloperSettings.h"
#include "LocalSettings.generated.h"

enum class ECommonInputType : uint8;
class ULocalPlayerCustom;
class UPlayerMappableInputConfig;
class USoundControlBus;
class USoundControlBusMix;
class UBindingConfiguration;
class UInputMappingContext;

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

USTRUCT()
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input", meta=(AssetBundles="Client,Server"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditAnywhere, Category="Input")
	int32 Priority = 0;
		
	/** If true, then this mapping context will be registered with the settings when this game feature action is registered. */
	UPROPERTY(EditAnywhere, Category="Input")
	bool bRegisterWithSettings = true;
};


UCLASS(config = Game, defaultconfig, meta = (DisplayName = "InputBindngs"))
class MENUSETTINGS_API UInputBindings final : public UDeveloperSettings
{
	GENERATED_BODY()

public :
	
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (AllowedClasses = "/Script/MenuSettings.InputMappingContext"))
	TArray<FInputMappingContextAndPriority> InputMappingContexts;
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
	/** Registers owned Input Mapping Contexts to the Input Registry Subsystem for a specified Local Player. This also gets called when a Local Player is added. */
	void RegisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer);
	
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