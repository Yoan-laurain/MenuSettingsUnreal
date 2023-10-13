#pragma once

#include "../../../Category/GameSettingsCollection.h"
#include "LanguageConfiguration.generated.h"

UCLASS()
class ULanguageConfiguration final : public UGameSettingsItem
{
	GENERATED_BODY()
	
public:

	// UGameSettingsItem implementation Begin
	virtual void ResetToDefault() override;
	void RestoreToInitial();
	// UGameSettingsItem implementation End

	/* Fill the AvailableCultureNames array with all the culture names configured in the project */
	void InitializeCultureNames();

	/* Retrieve all the culture names configured in the project */
	TArray<FString> GetAvailableCultureNames();

private :

	TArray<FString> AvailableCultureNames;
};