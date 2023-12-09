#include "CustomInputConfig.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CustomInputConfig)

UCustomInputConfig::UCustomInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UCustomInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FCustomInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	
	return nullptr;
}

const UInputAction* UCustomInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FCustomInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}
