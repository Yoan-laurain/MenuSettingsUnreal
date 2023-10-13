#include "ValidationPopUpWidget.h"
#include "Components/TextBlock.h"
#include "../../MenuSettingsWidget.h"

void UValidationPopUpWidget::OnValidateButtonClicked()
{
	HandleButtonClick(true);
}

void UValidationPopUpWidget::OnCancelButtonClicked()
{
	HandleButtonClick(false);
}

void UValidationPopUpWidget::HandleButtonClick(const bool bIsValidateButton)
{
	if (MenuSettingsWidget)
	{
		if (bIsValidateButton)
		{
			MenuSettingsWidget->ApplySettings();
		}
		else
		{
			MenuSettingsWidget->Cancel();
		}

		if (bShouldCloseMenuSettingsWidget)
		{
			MenuSettingsWidget->Close();
		}
		else
		{
			MenuSettingsWidget->SetFocusInternal();
		}

		MenuSettingsWidget->ActivateWidget();
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

void UValidationPopUpWidget::SetShouldCloseMenuSettingsWidget(const bool bInShouldCloseMenuSettingsWidget)
{
	bShouldCloseMenuSettingsWidget = bInShouldCloseMenuSettingsWidget;
}