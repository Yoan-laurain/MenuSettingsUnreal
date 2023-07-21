#pragma once

#include "CommonInputBaseTypes.h"
#include "CommonInputTypeEnum.h"
#include "GameplayTagContainer.h"
#include "UObject/SoftObjectPtr.h"
#include "MappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

/** A container to organize loaded player mappable configs to their CommonUI input type */
USTRUCT(BlueprintType)
struct FLoadedMappableConfigPair
{
	GENERATED_BODY()

	FLoadedMappableConfigPair() = default;
	FLoadedMappableConfigPair(const UPlayerMappableInputConfig* InConfig, ECommonInputType InType, const bool InIsActive, const bool InIsDefault)
		: Config(InConfig)
		, Type(InType)
		, bIsActive(InIsActive),
		bIsDefault(InIsDefault)
	{}

	/** The player mappable input config that should be applied to the Enhanced Input subsystem */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<const UPlayerMappableInputConfig> Config = nullptr;

	/** The type of device that this mapping config should be applied to */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	/** If this config is currently active. A config is marked as active when it's owning GFA is active */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsActive = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsDefault = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bUseThisConfig = false;
};

/** A container to organize potentially unloaded player mappable configs to their CommonUI input type */
USTRUCT()
struct FMappableConfigPair
{
	GENERATED_BODY()
	
	FMappableConfigPair() = default;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	/**
	 * The type of config that this is. Useful for filtering out configs by the current input device
	 * for things like the settings screen, or if you only want to apply this config when a certain
	 * input type is being used.
	 */
	UPROPERTY(EditAnywhere)
	ECommonInputType Type = ECommonInputType::Count;
	

	/** If true, then this input config will be activated when it's associated Game Feature is activated.
	 * This is normally the desirable behavior
	 */
	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
	
	UPROPERTY(EditAnywhere)
	bool bIsDefault = false;

	UPROPERTY(EditAnywhere)
	bool bUseThisConfig = false;

	/**
	 * Registers the given config mapping with the local settings
	 */
	static bool RegisterPair(const FMappableConfigPair& Pair);

	/**
	 * Unregisters the given config mapping with the local settings
	 */
	static void UnregisterPair(const FMappableConfigPair& Pair);
};

