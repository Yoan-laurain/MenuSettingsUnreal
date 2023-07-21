#include "BindingKeyWidget.h"

#include "ChooseAKeyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBindingKeyWidget::ClearKeyName()
{
	if ( CurrentValue )
	{
		CurrentValue->SetText(FText::FromString("None"));
	}
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
	ClearKeyName();
	
	if ( ChooseAKeyWidgetClass )
	{
		ChooseAKeyWidget = CreateWidget<UChooseAKeyWidget>(GetWorld(), ChooseAKeyWidgetClass);
		if ( ChooseAKeyWidget )
		{
			ChooseAKeyWidget->AddToViewport();
			ChooseAKeyWidget->SetFocus();
			ChooseAKeyWidget->Message->SetText(FText::FromString("Press any key to bind"));
		}
	}
}