#include "BindingKeyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBindingKeyWidget::InitWidget(const FText NewActionName, const FText NewKeyName)
{
	if ( this->ActionName )
	{
		this->ActionName->SetText(NewActionName);
	}
	
	if ( this->KeyName )
	{
		this->KeyName->SetText(NewKeyName);
	}
}

void UBindingKeyWidget::ClearKeyName()
{
	if ( KeyName )
	{
		KeyName->SetText(FText::FromString("None"));
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
	// TODO: Implement
}