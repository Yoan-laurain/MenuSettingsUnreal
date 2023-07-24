#include "ValidationPopUpWidget.h"
#include "Components/Button.h"
#include "MenuSettings/UI/Settings/Widget/MenuSettingsWidget.h"

void UValidationPopUpWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ValidateButton)
	{
		ValidateButton->OnClicked.AddDynamic(this, &UValidationPopUpWidget::OnValidateButtonClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UValidationPopUpWidget::OnCancelButtonClicked);
	}
}

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