#pragma once

#include "GameFramework/GameUserSettings.h"
#include "GeneralSettings.generated.h"

class UGameSettingsCollection;

UCLASS(config=Game, defaultconfig)
class MENUSETTINGS_API UGeneralSettings : public UGameUserSettings
{
public:

	GENERATED_BODY()
	UGeneralSettings();

	/** Returns true if this platform can run the auto benchmark */
	UFUNCTION(BlueprintCallable, Category = Settings)
	bool CanRunAutoBenchmark() const;

	/** Returns true if this user should run the auto benchmark as it has never been run */
	UFUNCTION(BlueprintCallable, Category = Settings)
	bool ShouldRunAutoBenchmarkAtStartup() const;

	/** Run the auto benchmark, optionally saving right away */
	UFUNCTION(BlueprintCallable, Category = Settings)
	void RunAutoBenchmark(bool bSaveImmediately);

	/** Apply just the quality scalability settings */
	void ApplyScalabilitySettings();
	
	static UGeneralSettings* Get();
	
	UGameSettingsCollection* InitializeVideoSettings();
private :

	// Does the platform support running the automatic quality benchmark (typically this should only be true if bSupportsGranularVideoQualitySettings is also true)
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsAutomaticVideoQualityBenchmark = true;
	
};