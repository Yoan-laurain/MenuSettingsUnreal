#include "LocalPlayerCustom.h"
#include "LocalSettings.h"

ULocalPlayerCustom::ULocalPlayerCustom()
: Super(FObjectInitializer::Get())
{
}

ULocalSettings* ULocalPlayerCustom::GetLocalSettings() const
{
	return ULocalSettings::Get();
}