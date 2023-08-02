#pragma once

#include <functional>
#include "../../../ProjectEnumerations.h"
#include "GameSettingsCollection.generated.h"

class UiSettingsParentClass;
class UGameSettingsItem;
class USettingsWidget;

UCLASS()
class MENUSETTINGS_API UGameSettingsItem : public UObject
{
	GENERATED_BODY()
	
public:

	UGameSettingsItem();

#pragma region GettersSetters
	
	FText GetOptionName() const { return OptionsName; }
	void SetOptionName(const FText& Value) { OptionsName = Value; }

	int GetDefaultOption() const { return IndexDefaultOption; }
	void SetDefaultOption(const int Value) { IndexDefaultOption = Value; }

	FText GetDescriptionRichText() const { return DescriptionRichText; }
	void SetDescriptionRichText(const FText& Value) { DescriptionRichText = Value; }

	TArray<FText> GetOptions() const { return Options; }
	void SetOptions(const TArray<FText>& Value) { Options = Value; }

	int GetTechnicalOption() const { return TechnicalOption[IndexCurrentOption]; }
	void SetTechnicalOption(const TArray<int>& Value) { TechnicalOption = Value; }

	int GetIndexCurrentOption() const { return IndexCurrentOption; }
	void SetIndexCurrentOption(const int Value)
	{
		IndexCurrentOption = Value;

		if ( IndexInitialOption == 0 ) // Set initial index if it is not set yet
		{
			IndexInitialOption = Value;
		}
	}

	TArray<UGameSettingsItem*> GetParentOptions() const { return ParentOptions; }
	void SetParentOptions(const TArray<UGameSettingsItem*>& Value) { ParentOptions = Value; }

	ESettingsType GetWidgetType() const { return WidgetType; }
	void SetWidgetType(const ESettingsType Value) { WidgetType = Value; }

	int GetParentUniqueOption() const { return ParentUniqueBaseOption; }
	void SetParentUniqueOption(const int Value) { ParentUniqueBaseOption = Value; }

	void SetInitialIndex(const int Value) { IndexInitialOption = Value; }
	int GetInitialIndex() const { return IndexInitialOption; }

#pragma endregion GettersSetters

	/* Increment the index of the current option */
	void SetNextTechnicalOption()
	{
		if (  TechnicalOption.Num() - 1  != IndexCurrentOption )
		{
			IndexCurrentOption++;
		}
	}

	/* Decrement the index of the current option */
	void SetPreviousTechnicalOption()
	{
		if ( 0 != IndexCurrentOption )
		{
			IndexCurrentOption--;
		}
	}

	virtual bool ValueHasChanged();

	FIntPoint ConvertIntToFIntPoint(int Value);
	int ConvertFIntPointToInt(const FIntPoint& Value);

	/* Set the base options ( Default ( Low, Medium, High, Epic ) ) */
	void SetBaseOptions();

	/* Clear the list of options */
	void ClearOptions() { Options.Empty(); }

	/* Retrieve the current option selected */
	FText GetCurrentOption() const { return Options[IndexCurrentOption]; }

	/* Add an option to the option list */
	void AddOption(const FText& Option) { Options.Add(Option); }

	/* Used for button ( increased / decreased ) to know if they should be enabled or not  */ 
	bool IsMaxValue() const { return IndexCurrentOption == Options.Num() - 1; }
	bool IsMinValue() const { return IndexCurrentOption == 0; }

	/* Retrieve the delegate */
	DECLARE_DELEGATE(FSetCurrentOptionValueDelegate)
	FSetCurrentOptionValueDelegate& GetCurrentOptionValueDelegate();

	/* Delegate to execute the current option value */
	void ExecCurrentOptionValueDelegate();
	
	/* Add a dependent option to this option */
	void AddDependentOption(UGameSettingsItem* DependentOption);

	/* Reset the current index to the initial index */
	virtual void CancelChanges();

	/* Reset the current index to the default index */
	virtual void ResetToDefault();

	/* The widget associated with this option */
	UiSettingsParentClass* GetWidget() const { return Widget; }
	void SetWidget(UiSettingsParentClass* Value) { Widget = Value; }

	/* Retrieve the options that are dependent on this option */
	TArray<UGameSettingsItem*> GetDependentOptions() const { return DependentOptions; }

	/* The value that is currently stored in local in .sav */ 
	int GetIndexFromFile() const;
	void SetMethodToGetIndexFromFile(std::function<int()> Method);

private:

	/* The name of the option */ 
	FText OptionsName;

	/* The description of the option */
	FText DescriptionRichText;

	/* The options that this option can have */
	TArray<FText> Options;

	/* Used to know what type of widget to used for this option */
	ESettingsType WidgetType;

	/* Option that should change if this option changes */
	TArray<UGameSettingsItem*> DependentOptions;

	/* The options that this option is dependent on */
	TArray<UGameSettingsItem*> ParentOptions;

	int IndexCurrentOption;
	int IndexInitialOption;
	int IndexDefaultOption;

	/* The index to set the parent when the option is changed */
	int ParentUniqueBaseOption;

	/* Array of indexes used to know witch value to set in the .sav file */
	TArray<int> TechnicalOption; 

	/* The delegate to call when the option is changed ( Apply the setting in the game but do not save it ) */
	DECLARE_DELEGATE(FSetCurrentOptionValueDelegate)
	FSetCurrentOptionValueDelegate CurrentOptionValueDelegateSet;

	/* Retrieve the index from the .sav file */
	std::function<int()> MethodToGetIndexFromFile;

	/* The widget associated with this option */
	UPROPERTY()
	UiSettingsParentClass* Widget;
};

UCLASS(config=Game, defaultconfig)
class MENUSETTINGS_API UGameSettingsCollection final : public UGameSettingsItem
{
	GENERATED_BODY()
	
public:
	
	TArray<UGameSettingsItem*> GetChildSettings() { return Settings; }
	TArray<UGameSettingsCollection*> GetChildSettingsCollection() { return SettingsCollection; }

	/* Add a setting to this category */
	void AddSetting(UGameSettingsItem* Setting);

	/* A a category to this category */
	void AddSettingCollection(UGameSettingsCollection* SettingCollection);

	FText GetTitle() const { return Title; }
	void SetTitle(const FText& Value) { Title = Value; }

protected :

	/* All the settings of this category ( Potentially empty if we have more categories ) */
	UPROPERTY()
	TArray<UGameSettingsItem*> Settings;

	/* All the categories of this categories ( Potentially empty if we have settings ) */
	UPROPERTY()
	TArray<UGameSettingsCollection*> SettingsCollection;

	/* The title of this category */
	FText Title;
};