#include "ValidationPopUpWidget.h"

#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/Widget/MenuSettingsWidget.h"

void UValidationPopUpWidget::OnValidateButtonClicked()
{
	if (MenuSettingsWidget)
	{
		MenuSettingsWidget->ApplySettings();
		RemoveFromParent();
	}
}

void UValidationPopUpWidget::OnCancelButtonClicked()
{
	if (MenuSettingsWidget)
	{
		MenuSettingsWidget->Cancel();
		RemoveFromParent();
	}
}

void UValidationPopUpWidget::SetMenuSettingsWidget(UMenuSettingsWidget* InMenuSettingsWidget)
{
	MenuSettingsWidget = InMenuSettingsWidget;
}

void UValidationPopUpWidget::SetTitleText(const FText& InTitleText)
{
	if (TitleText)
	{
		TitleText->SetText(InTitleText);
	}
}