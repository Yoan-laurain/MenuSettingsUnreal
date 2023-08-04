#pragma once

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

#pragma region GetterSetters

	FText GetPrimaryKeyText() const;
	FText GetSecondaryKeyText() const;

	FKeyboardOption& GetFirstMappableOption() { return FirstMappableOption; }
	FKeyboardOption& GetSecondaryMappableOption() { return SecondaryMappableOption; }

	FText GetSettingDisplayName() const { return FirstMappableOption.InputMapping.PlayerMappableOptions.DisplayName; }

#pragma endregion GetterSetters

	/** Initalize this setting widget based off the given mapping */
	void SetInputData(FEnhancedActionKeyMapping& BaseMapping, const UPlayerMappableInputConfig* InOwningConfig, int32 InKeyBindSlot);
	
	virtual void StoreInitial();

	/* Restore all mapping to default value */
	virtual void ResetToDefault() override;

	/* Restore all mapping to initial value */
	virtual void RestoreToInitial();

	/* Change the binding of the given key bind slot to the given key */
	bool ChangeBinding(int32 InKeyBindSlot, FKey NewKey);

	/* Get all actions that are mapped to the given key */
	void GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key, TArray<FName>& OutActionNames) const;

	/* Set the bindings to null */
	void ClearBindings();

	/* Cancel any changes made to the bindings */
	virtual void CancelChanges() override;

	virtual bool ValueHasChangedCompareToStart() override;
	virtual bool ValueHasChangedCompareToDefault() override;

	void SetIsKeyboard(bool InIsKeyboard) { isKeyboard = InIsKeyboard; }
	bool GetIsKeyboard() const { return isKeyboard; }

	void RegisterNewBinding(const FKey& InKey);

private :

	bool isKeyboard;

protected:

	FKeyboardOption FirstMappableOption;
	FKeyboardOption SecondaryMappableOption;
};