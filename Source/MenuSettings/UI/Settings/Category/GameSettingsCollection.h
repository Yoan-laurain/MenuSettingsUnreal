#pragma once
#include "CoreMinimal.h"
#include "GameSettingsCollection.generated.h"

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
		if (  TechnicalOption<TypeOption>.Num() - 1  != IndexCurrentOption )
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

	template <typename TypeOption>
	TypeOption GetTechnicalOption() const { return TechnicalOption<TypeOption>[IndexCurrentOption]; }
	
	template <typename TypeOption>
	void SetTechnicalOption(const TArray<TypeOption> Value) { TechnicalOption<TypeOption> = Value; }

	int GetIndexCurrentOption() const { return IndexCurrentOption; }
	void SetIndexCurrentOption(const int Value) { IndexCurrentOption = Value; }

	UGameSettingsItem* GetParentOption() const { return ParentOption; }
	void SetParentOption(UGameSettingsItem* Value) { ParentOption = Value; }

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

	void CancelChanges();

	void AddDependentOption(UGameSettingsItem* DependentOption);
	
	USettingsWidget* GetWidget() const { return Widget; }
	void SetWidget(USettingsWidget* Value) { Widget = Value; }
	
	TArray<UGameSettingsItem*> GetDependentOptions() const { return DependentOptions; }
	
private:
	
	FText OptionsName;
	FText DescriptionRichText;
	TArray<FText> Options;

	UPROPERTY()
	TArray<UGameSettingsItem*> DependentOptions;

	UPROPERTY()
	UGameSettingsItem* ParentOption;

	int IndexCurrentOption;

	template <typename TypeOption>
	TArray<TypeOption> TechnicalOption; 
	
	FSetCurrentOptionValueDelegate CurrentOptionValueDelegateSet;
	
	UPROPERTY()
	USettingsWidget* Widget;
	
};

UCLASS(config=Game, defaultconfig)
class MENUSETTINGS_API UGameSettingsCollection final : public UGameSettingsItem
{
	GENERATED_BODY()
	
public:
	UGameSettingsCollection();
	
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