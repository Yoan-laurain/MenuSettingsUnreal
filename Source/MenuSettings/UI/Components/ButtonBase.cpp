#include "ButtonBase.h"
#include "CommonActionWidget.h"
#include "CommonInputTypeEnum.h"

void UButtonBase::SetButtonText(const FText& InText)
{
	bOverride_ButtonText = InText.IsEmpty();
	ButtonText = InText;
	RefreshButtonText();
}

void UButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();

	UpdateButtonStyle();
	RefreshButtonText();
}

void UButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);

	UpdateButtonStyle();
}

void UButtonBase::RefreshButtonText()
{
	if (bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if (InputActionWidget)
		{
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();	
			if (!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}
	
	UpdateButtonText(ButtonText);	
}