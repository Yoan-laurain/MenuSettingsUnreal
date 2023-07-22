#include "BindingKeyWidget.h"

#include "ChooseAKeyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/Category/Mouse&Keyboard/Configuration/BindingConfiguration.h"

void UBindingKeyWidget::Refresh()
{
	const UBindingConfiguration* Item = Cast<UBindingConfiguration>(GetSettingsItem());
	CurrentValue->SetText(Item->GetPrimaryKeyText());
}

void UBindingKeyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if ( Button )
	{
		Button->OnClicked.AddDynamic(this, &UBindingKeyWidget::OnTryKeyChange);
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