#pragma once

#include "Engine/LocalPlayer.h"
#include "LocalPlayerCustom.generated.h"

class ULocalSettings;
class UInputMappingContext;

UCLASS()
class ULocalPlayerCustom : public ULocalPlayer
{
	GENERATED_BODY()
	
public:

	ULocalPlayerCustom();

	UFUNCTION()
	ULocalSettings* GetLocalSettings() const;
	
private :

	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;
};