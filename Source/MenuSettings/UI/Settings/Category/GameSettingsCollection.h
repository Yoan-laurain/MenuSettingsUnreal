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
	void SetOptionName(const FText Value) { OptionsName = Value; }

	FText GetDescriptionRichText() const { return DescriptionRichText; }
	void SetDescriptionRichText(const FText Value) { DescriptionRichText = Value; }

	TArray<FText> GetOptions() const { return Options; }
	void SetOptions(const TArray<FText> Value) { Options = Value; }

	template <typename TypeOption>
	void SetNextTechnicalOption()
	{
		if (  TechnicalOption.Num() - 1  != IndexCurrentOption )
		{
			IndexCurrentOption++;
		}
	}
	
	template <typename TypeOption>
	void SetPreviousTechnicalOption()
	{
		if ( 0 != IndexCurrentOption )
		{
			IndexCurrentOption--;
		}
	}

	FIntPoint ConvertIntToFIntPoint(int Value);
	int ConvertFIntPointToInt(FIntPoint Value);
	
	int GetTechnicalOption() const { return TechnicalOption[IndexCurrentOption]; }
	void SetTechnicalOption(const TArray<int> Value) { TechnicalOption = Value; }

	int GetIndexCurrentOption() const { return IndexCurrentOption; }
	void SetIndexCurrentOption(const int Value)
	{
		IndexCurrentOption = Value;

		if ( BaseOption == 0 )
		{
			BaseOption = Value;
		}
	}

	int GetBaseOption() const { return BaseOption; }
	void SetBaseOption(const int Value) { BaseOption = Value; }

	TArray<UGameSettingsItem*> GetParentOptions() const { return ParentOptions; }
	void SetParentOptions(const TArray<UGameSettingsItem*> Value) { ParentOptions = Value; }

	void SetType(const ESettingsType Value) { Type = Value; }
	ESettingsType GetType() const { return Type; }

	void SetParentUniqueOption(const int Value) { ParentUniqueBaseOption = Value; }
	int GetParentUniqueOption() const { return ParentUniqueBaseOption; }

#pragma endregion GettersSetters

#pragma region Options

	void SetBaseOptions();
	void ClearOptions() { Options.Empty(); }
	
	void AddOption(const FText Option) { Options.Add(Option); }
	FText GetCurrentOption() const { return Options[IndexCurrentOption]; }
	FText GetPreviousOptions(const FText& CurrentOption);
	FText GetNextOptions(const FText CurrentOption);
	
	bool IsMaxValue(const FText& CurrentOption) const { return Options[Options.Num() - 1].EqualTo(CurrentOption); }
	bool IsMinValue(const FText& CurrentOption) const { return Options[0].EqualTo(CurrentOption); }

#pragma endregion Options
	
	DECLARE_DELEGATE(FSetCurrentOptionValueDelegate)
	FSetCurrentOptionValueDelegate& GetCurrentOptionValueDelegate();

	void ExecCurrentOptionValueDelegate();
	virtual void CancelChanges();
	void AddDependentOption(UGameSettingsItem* DependentOption);
	
	UiSettingsParentClass* GetWidget() const { return Widget; }
	void SetWidget(UiSettingsParentClass* Value) { Widget = Value; }
	
	TArray<UGameSettingsItem*> GetDependentOptions() const { return DependentOptions; }

	int GetIndexFromFile() const;
	
	void SetMethodToGetIndexFromFile(std::function<int()> Method);

private:
	
	FText OptionsName;
	FText DescriptionRichText;
	TArray<FText> Options;

	ESettingsType Type;
	
	TArray<UGameSettingsItem*> DependentOptions;
	TArray<UGameSettingsItem*> ParentOptions;

	int IndexCurrentOption;
	int BaseOption;
	int ParentUniqueBaseOption;
	
	TArray<int> TechnicalOption; 
	
	DECLARE_DELEGATE(FSetCurrentOptionValueDelegate)
	FSetCurrentOptionValueDelegate CurrentOptionValueDelegateSet;

	std::function<int()> MethodToGetIndexFromFile;
	
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
	
	void AddSetting(UGameSettingsItem* Setting);
	void AddSettingCollection(UGameSettingsCollection* SettingCollection);

	FText GetTitle() const { return Title; }
	void SetTitle(const FText Value) { Title = Value; }

protected :

	UPROPERTY()
	TArray<UGameSettingsItem*> Settings;

	UPROPERTY()
	TArray<UGameSettingsCollection*> SettingsCollection;
	
	FText Title;
};