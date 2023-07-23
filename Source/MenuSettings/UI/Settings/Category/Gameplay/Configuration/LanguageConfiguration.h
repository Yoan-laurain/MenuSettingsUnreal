#pragma once

#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "LanguageConfiguration.generated.h"

UCLASS()
class ULanguageConfiguration final : public UGameSettingsItem
{
	GENERATED_BODY()
public:
	
	void InitializeCultureNames();

	virtual void ResetToDefault() override;
	void RestoreToInitial();
	TArray<FString> GetAvailableCultureNames();

private :

	TArray<FString> AvailableCultureNames;
};