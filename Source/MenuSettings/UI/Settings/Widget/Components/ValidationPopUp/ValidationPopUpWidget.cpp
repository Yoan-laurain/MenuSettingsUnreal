#include "ValidationPopUpWidget.h"
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
	RemoveFromParent();
}

void UValidationPopUpWidget::SetMenuSettingsWidget(UMenuSettingsWidget* InMenuSettingsWidget)
{
	MenuSettingsWidget = InMenuSettingsWidget;
}