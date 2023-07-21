#pragma once

#include "Engine/DataAsset.h"
#include "InputDataAsset.generated.h"

struct FMappableConfigPair;

UCLASS()
class MENUSETTINGS_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public :
	
	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> InputConfigs;
};