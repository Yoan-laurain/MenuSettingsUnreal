#pragma once

#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "GameSettingDataSource.h"
#include "PropertyPathHelpers.h"

class ULocalPlayer;

//--------------------------------------
// FGameSettingDataSourceDynamic
//--------------------------------------

class MENUSETTINGS_API FGameSettingDataSourceDynamic : public FGameSettingDataSource
{
public:
	FGameSettingDataSourceDynamic(const TArray<FString>& InDynamicPath);
	
 	FString GetValueAsString(ULocalPlayer* InLocalPlayer) const override;
 	void SetValue(ULocalPlayer* InLocalPlayer, const FString& Value) override;

private:
 	FCachedPropertyPath DynamicPath;
};