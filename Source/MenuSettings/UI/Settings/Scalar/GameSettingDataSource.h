#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Delegates/Delegate.h"

class ULocalPlayer;

class MENUSETTINGS_API FGameSettingDataSource : public TSharedFromThis<FGameSettingDataSource>
{
public:
	virtual ~FGameSettingDataSource() { }

	/**
	 * Some settings may take an async amount of time to finish initializing.  The settings system will wait
	 * for all settings to be ready before showing the setting.
	 */
	virtual void Startup(ULocalPlayer* InLocalPlayer, FSimpleDelegate StartupCompleteCallback) { StartupCompleteCallback.ExecuteIfBound(); }

	virtual FString GetValueAsString(ULocalPlayer* InContext) const = 0;
	
	virtual void SetValue(ULocalPlayer* InContext, const FString& Value) = 0;
};
