#include "BindingKeyWidget.h"
#include "ChooseAKeyWidget.h"
#include "../../../../Components/ButtonBase.h"
#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/Category/Mouse&Keyboard/Configuration/BindingConfiguration.h"
#include "MenuSettings/UI/Settings/Widget/MenuSettingsWidget.h"

void UBindingKeyWidget::Refresh()
{
	const UBindingConfiguration* Item = Cast<UBindingConfiguration>(GetSettingsItem());
	SetCurrentValue(Item->GetPrimaryKeyText());
}

void UBindingKeyWidget::UpdateHUD()
{
	Refresh();
}

void UBindingKeyWidget::SetCurrentValue(const FText NewText)
{
	Button->SetButtonText(NewText);

	if ( ParentWidget )
	{
		ParentWidget->OnSettingsDirtyStateChanged_Implementation(true);
	}
}

void UBindingKeyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if ( Button )
	{
		Button->OnClicked().AddUObject(this, &UBindingKeyWidget::OnTryKeyChange);
	}
}

void UBindingKeyWidget::OnTryKeyChange()
{
	if ( ChooseAKeyWidgetClass )
	{
		ChooseAKeyWidget = CreateWidget<UChooseAKeyWidget>(GetWorld(), ChooseAKeyWidgetClass);
		if ( ChooseAKeyWidget )
		{
			ChooseAKeyWidget->AddToViewport();
			ChooseAKeyWidget->SetFocus();
			ChooseAKeyWidget->SetParent(this);
			ChooseAKeyWidget->Message->SetText(ChooseAKeyWidget->GetMessagesToDisplay()[0]);
		}
	}
}