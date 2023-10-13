#pragma once

#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "../UI/Settings/LocalSettings.h"
#include "LocalPlayerCustom.generated.h"

class ULocalSettings;

UCLASS()
class ULocalPlayerCustom final : public ULocalPlayer
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	static ULocalSettings* GetLocalSettings()
	{
		return ULocalSettings::Get();
	}
	
private :

	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;
};