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
	
	bool IsSelectable() const { return true; }

	TArray<FString> GetOptions() const { return Options; }
	void SetOptions(TArray<FString> Value) { Options = Value; }
	
	void SetBaseOptions();
	void ClearOptions() { Options.Empty(); }
	
	void AddOption(FString Option) { Options.Add(Option); }

private:
	FText NavigationText;
	FText DescriptionRichText;
	TArray<FString> Options;
};

UCLASS(config=Game, defaultconfig)
class MENUSETTINGS_API UGameSettingsCollection : public UGameSettingsItem
{
	GENERATED_BODY()
	
public:
	UGameSettingsCollection();
	
	TArray<UGameSettingsItem*> GetChildSettings() { return Settings; }
	void AddSetting(UGameSettingsItem* Setting);
	bool IsSelectable() const { return false; }

	FText GetTitle() const { return Title; }
	void SetTitle(FText Value) { Title = Value; }

protected :
	TArray<UGameSettingsItem*> Settings;
	FText Title;
};