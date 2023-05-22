#pragma once

#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreTypes.h"
#include "GameSettingValue.h"
#include "Internationalization/Text.h"
#include "Misc/AssertionMacros.h"
#include "GameSettingValueDiscrete.generated.h"

class UObject;
struct FFrame;

UCLASS(Abstract)
class MENUSETTINGS_API UGameSettingValueDiscrete : public UGameSettingValue
{
	GENERATED_BODY()

public:
	UGameSettingValueDiscrete();

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) PURE_VIRTUAL(,);
	
	UFUNCTION(BlueprintCallable)
	virtual int32 GetDiscreteOptionIndex() const PURE_VIRTUAL(,return INDEX_NONE;);

	/** Optional */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetDiscreteOptionDefaultIndex() const { return INDEX_NONE; }

	UFUNCTION(BlueprintCallable)
	virtual TArray<FText> GetDiscreteOptions() const PURE_VIRTUAL(,return TArray<FText>();); 

	virtual FString GetAnalyticsValue() const;
};

inline UGameSettingValueDiscrete::UGameSettingValueDiscrete()
{
}

inline FString UGameSettingValueDiscrete::GetAnalyticsValue() const
{
	const TArray<FText> NewOptions = GetDiscreteOptions();
	const int32 CurrentOptionIndex = GetDiscreteOptionIndex();
	if (NewOptions.IsValidIndex(CurrentOptionIndex))
	{
		const FString* SourceString = FTextInspector::GetSourceString(NewOptions[CurrentOptionIndex]);
		if (SourceString)
		{
			return *SourceString;
		}
	}

	return TEXT("<Unknown Index>");
}
