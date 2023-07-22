﻿#pragma once

#include "EnhancedActionKeyMapping.h"
#include "../../GameSettingsCollection.h"
#include "BindingConfiguration.generated.h"

class UPlayerMappableInputConfig;

struct FKeyboardOption
{
	FKeyboardOption() = default;
	
	FEnhancedActionKeyMapping InputMapping {};
	
	const UPlayerMappableInputConfig* OwningConfig = nullptr;

	void ResetToDefault();

	/** Store the currently set FKey that this is bound to */
	void SetInitialValue(FKey InKey);

	/** Get the most recently stored initial value */
	FKey GetInitialStoredValue() const { return InitialMapping; };

private:

	/** The key that this option is bound to initially, used in case the user wants to cancel their mapping */
	FKey InitialMapping;
};

UCLASS()
class UBindingConfiguration : public UGameSettingsItem
{
	GENERATED_BODY()

public:

	/** Initalize this setting widget based off the given mapping */
	void SetInputData(FEnhancedActionKeyMapping& BaseMapping, const UPlayerMappableInputConfig* InOwningConfig, int32 InKeyBindSlot);

	FText GetPrimaryKeyText() const;
	FText GetSecondaryKeyText() const;
	
	virtual void StoreInitial();

	/* Restore all mapping to default value */
	virtual void ResetToDefault();

	/* Restore all mapping to initial value */
	virtual void RestoreToInitial();

	bool ChangeBinding(int32 InKeyBindSlot, FKey NewKey);
	void GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key, TArray<FName>& OutActionNames) const;

	FKeyboardOption& GetFirstMappableOption() { return FirstMappableOption; }
	FKeyboardOption& GetSecondaryMappableOption() { return SecondaryMappableOption; }

	void Clear();
	virtual void CancelChanges() override;
	
	FText GetSettingDisplayName() const { return FirstMappableOption.InputMapping.PlayerMappableOptions.DisplayName; }

protected:

	FKeyboardOption FirstMappableOption;
	FKeyboardOption SecondaryMappableOption;
};