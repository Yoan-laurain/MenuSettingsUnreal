#pragma once

#include "MenuSettings/UI/Settings/GameSettingsCollection.h"
#include "Misc/AssertionMacros.h"
#include "GameSettingValue.generated.h"

class UObject;

UCLASS(Abstract)
class MENUSETTINGS_API UGameSettingValue : public UGameSettingsItem
{
	GENERATED_BODY()

public:
	UGameSettingValue();

	/** Stores an initial value for the setting.  This will be called on initialize, but should also be called if you 'apply' the setting. */
	virtual void StoreInitial() PURE_VIRTUAL(, );

	/** Resets the property to the default. */
	virtual void ResetToDefault() PURE_VIRTUAL(, );

	/** Restores the setting to the initial value, this is the value when you open the settings before making any tweaks. */
	virtual void RestoreToInitial() PURE_VIRTUAL(, );

protected:
	virtual void OnInitialized();
};
