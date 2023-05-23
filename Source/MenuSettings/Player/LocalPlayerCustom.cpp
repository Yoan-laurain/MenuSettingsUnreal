#include "LocalPlayerCustom.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"

ULocalPlayerCustom::ULocalPlayerCustom()
: Super(FObjectInitializer::Get())
{
}

ULocalSettings* ULocalPlayerCustom::GetLocalSettings() const
{
	return ULocalSettings::Get();
}