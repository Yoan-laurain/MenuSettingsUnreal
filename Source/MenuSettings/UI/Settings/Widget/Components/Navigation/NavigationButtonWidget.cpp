#include "NavigationButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/Widget/MenuSettingsWidget.h"

void UNavigationButtonWidget::InitWidget(const FString Text)
{
	if ( NavigationText )
	{
		NavigationText->SetText(FText::FromString(Text));
	}

	if ( NavigationButton )
	{
		NavigationButton->OnClicked.AddDynamic(this, &UNavigationButtonWidget::OnNavigationButtonClicked);
	}
}

void UNavigationButtonWidget::OnNavigationButtonClicked()
{
	// ReSharper disable once CppExpressionWithoutSideEffects
	NavigationButtonClickedDelegate.ExecuteIfBound();
}
