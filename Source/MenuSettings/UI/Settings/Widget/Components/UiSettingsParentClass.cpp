#include "UiSettingsParentClass.h"
#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"

void UiSettingsParentClass::SetSettingsText(FText NewText)
{
	if ( SettingsText )
	{
		SettingsText->SetText(NewText);
	}
}

void UiSettingsParentClass::SetCurrentValue(FText NewText)
{
	if ( CurrentValue )
	{
		CurrentValue->SetText(NewText);
	}
}

void UiSettingsParentClass::InitWidget(UGameSettingsItem* NewSettingsItem)
{
	SettingsItem = NewSettingsItem;
	SetSettingsText(SettingsItem->GetOptionName());
	SetCurrentValue( SettingsItem->GetCurrentOption() ); 
}

void UiSettingsParentClass::ApplySetting()
{
	SettingsItem->ExecCurrentOptionValueDelegate();
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	if ( LocalSettings )
	{
		LocalSettings->ApplySettings(false);
	}
}

void UiSettingsParentClass::SetParentWidget(UMenuSettingsWidget* NewParentWidget)
{
	ParentWidget = NewParentWidget;
}
