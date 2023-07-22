#include "UiSettingsParentClass.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../LocalSettings.h"
#include "../../Category/GameSettingsCollection.h"
#include "../MenuSettingsWidget.h"

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
	SettingsItem->SetWidget(this);
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

void UiSettingsParentClass::SetStateButtons()
{
}

void UiSettingsParentClass::UpdateHUD()
{
}

UGameSettingsItem* UiSettingsParentClass::GetSettingsItem() const
{
	return SettingsItem;
}

void UiSettingsParentClass::OnHover()
{
	if (ParentWidget)
	{
		ParentWidget->ChangeDescription(SettingsItem->GetDescriptionRichText());
	}
}
