#include "GeneralSettings.h"

UGeneralSettings::UGeneralSettings()
{
}

bool UGeneralSettings::CanRunAutoBenchmark() const
{
	return bSupportsAutomaticVideoQualityBenchmark;
}

bool UGeneralSettings::ShouldRunAutoBenchmarkAtStartup() const
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

void UGeneralSettings::RunAutoBenchmark(bool bSaveImmediately)
{
	RunHardwareBenchmark();
	
	// Always apply, optionally save
	ApplyScalabilitySettings();

	if (bSaveImmediately)
	{
		SaveSettings();
	}
}

void UGeneralSettings::ApplyScalabilitySettings()
{
	Scalability::SetQualityLevels(ScalabilityQuality);
}

UGeneralSettings* UGeneralSettings::Get()
{
	return GEngine ? CastChecked<UGeneralSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void SettingChanged() 
{
	Cast<UGeneralSettings>(GEngine->GetGameUserSettings())->ApplyScalabilitySettings();
}