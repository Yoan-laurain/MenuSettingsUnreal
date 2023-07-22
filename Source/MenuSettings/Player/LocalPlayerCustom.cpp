#include "LocalPlayerCustom.h"
#include "../UI/Settings/LocalSettings.h"

ULocalSettings* ULocalPlayerCustom::GetLocalSettings() const
{
	return ULocalSettings::Get();
}