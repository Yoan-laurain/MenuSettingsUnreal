#pragma once

#include "GameSettingsCollection.generated.h"

class UGameSettingsItem;

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

	template<class  T>
	T GetCurrentValue() const { return CurrentValue<T>; }

	template<class  T>
	void SetCurrentValue(T Value) { CurrentValue<T> = Value; }

	template<class  M>
	M GetDefaultValue() const { return DefaultValue<M>; }

	template<class  M>
	void SetDefaultValue(M Value) { DefaultValue<M> = Value; }

#pragma endregion GettersSetters

#pragma region Options

	void SetBaseOptions();
	void ClearOptions() { Options.Empty(); }
	
	void AddOption(const FText Option) { Options.Add(Option); }
	FText GetDefaultOption() const { return Options[0]; }
	FText GetPreviousOptions(const FText& CurrentOption);
	FText GetNextOptions(const FText CurrentOption);
	
	bool IsMaxValue(const FText& CurrentOption) const { return Options[Options.Num() - 1].EqualTo(CurrentOption); }
	bool IsMinValue(const FText& CurrentOption) const { return Options[0].EqualTo(CurrentOption); }

#pragma endregion Options
	
	DECLARE_DELEGATE(FSetCurrentOptionValueDelegate)
	FSetCurrentOptionValueDelegate SetCurrentOptionValueDelegate();

private:
	
	FText OptionsName;
	FText DescriptionRichText;
	TArray<FText> Options;

	template<class T>
	inline static T CurrentValue = T();

	template<typename  M>
	inline static M DefaultValue = M();
	
	FSetCurrentOptionValueDelegate CurrentOptionValueDelegateSet;
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