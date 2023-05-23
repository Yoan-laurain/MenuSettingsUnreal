#include "../../LocalSettings.h"

FString ULocalSettings::GetDesiredDeviceProfileQualitySuffix() const
{
	return DesiredUserChosenDeviceProfileSuffix;
}

void ULocalSettings::SetDesiredDeviceProfileQualitySuffix(const FString& InSuffix)
{
	DesiredUserChosenDeviceProfileSuffix = InSuffix;
}