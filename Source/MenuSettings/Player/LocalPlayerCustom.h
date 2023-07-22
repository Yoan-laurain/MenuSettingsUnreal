#pragma once

#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "LocalPlayerCustom.generated.h"

class ULocalSettings;

UCLASS()
class ULocalPlayerCustom : public ULocalPlayer
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	ULocalSettings* GetLocalSettings() const;
	
private :

	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;
};