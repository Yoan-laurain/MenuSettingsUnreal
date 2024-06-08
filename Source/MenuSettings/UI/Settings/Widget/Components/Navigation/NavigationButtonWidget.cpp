#include "NavigationButtonWidget.h"
#include "CommonInputTypeEnum.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../../LocalSettings.h"
#include "../../../Category/GameSettingsCollection.h"

void UNavigationButtonWidget::InitWidget(const UGameSettingsCollection* Setting)
{
	if ( NavigationText )
	{
		NavigationText->SetText(Setting->GetTitle());
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