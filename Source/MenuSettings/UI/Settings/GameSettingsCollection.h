#pragma once

#include "GameSettingsCollection.generated.h"

class UGameSettingsItem;

UCLASS()
class MENUSETTINGS_API UGameSettingsItem : public UObject
{
	GENERATED_BODY()
public:

	UGameSettingsItem();
	
	FText GetNavigationText() const { return NavigationText; }
	void SetNavigationText(FText Value) { NavigationText = Value; }

	FText GetDescriptionRichText() const { return DescriptionRichText; }
	void SetDescriptionRichText(FText Value) { DescriptionRichText = Value; }

	TArray<FText> GetOptions() const { return Options; }
	void SetOptions(TArray<FText> Value) { Options = Value; }
	
	void SetBaseOptions();
	void ClearOptions() { Options.Empty(); }
	
	void AddOption(FText Option) { Options.Add(Option); }
	FText GetDefaultOption() const { return Options[0]; }
	FText GetPreviousOptions(const FText& CurrentOption);
	FText GetNextOptions(const FText CurrentOption);
	bool IsMaxValue(const FText& CurrentOption) const { return Options[Options.Num() - 1].EqualTo(CurrentOption); }
	bool IsMinValue(const FText& CurrentOption) const { return Options[0].EqualTo(CurrentOption); }

private:
	FText NavigationText;
	FText DescriptionRichText;
	TArray<FText> Options;
};

UCLASS(config=Game, defaultconfig)
class MENUSETTINGS_API UGameSettingsCollection : public UGameSettingsItem
{
	GENERATED_BODY()
	
public:
	UGameSettingsCollection();
	
	TArray<UGameSettingsItem*> GetChildSettings() { return Settings; }
	TArray<UGameSettingsCollection*> GetChildSettingsCollection() { return SettingsCollection; }
	
	void AddSetting(UGameSettingsItem* Setting);
	void AddSettingCollection(UGameSettingsCollection* SettingCollection);

	FText GetTitle() const { return Title; }
	void SetTitle(FText Value) { Title = Value; }

protected :

	UPROPERTY()
	TArray<UGameSettingsItem*> Settings;

	UPROPERTY()
	TArray<UGameSettingsCollection*> SettingsCollection;
	
	FText Title;
};