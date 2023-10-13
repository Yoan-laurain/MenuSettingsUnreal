#include "NavigationButtonWidget.h"
#include "CommonInputTypeEnum.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../../LocalSettings.h"

void UNavigationButtonWidget::InitWidget(const FString& Text)
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

	if ( ULocalSettings::Get()->CurrentInputType == ECommonInputType::Gamepad )
	{
		SetHoverState(true);
	}
}