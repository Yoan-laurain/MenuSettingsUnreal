#include "../../LocalSettings.h"

bool ULocalSettings::CanRunAutoBenchmark() const
{
	return bSupportsAutomaticVideoQualityBenchmark;
}

bool ULocalSettings::ShouldRunAutoBenchmarkAtStartup() const
{
	if (!CanRunAutoBenchmark())
	{
		return false;
	}

	if (LastCPUBenchmarkResult != -1)
	{
		// Already run and loaded
		return false;
	}

	return true;
}

void ULocalSettings::RunAutoBenchmark(bool bSaveImmediately)
{
	RunHardwareBenchmark();
	
	// Always apply, optionally save
	ApplyScalabilitySettings();

	if (bSaveImmediately)
	{
		SaveSettings();
	}
}

void ULocalSettings::ApplyScalabilitySettings()
{
	SetQualityLevels(ScalabilityQuality);
}

void SettingChanged() 
{
	Cast<ULocalSettings>(GEngine->GetGameUserSettings())->ApplyScalabilitySettings();
}