#include "NavigationButtonsContainer.h"
#include "CommonInputTypeEnum.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "../../../LocalSettings.h"
#include "NavigationButtonWidget.h"

void UNavigationButtonsContainer::AddNavigationButton(UNavigationButtonWidget* Button)
{
	NavigationButtonsBox->AddChild(Button);
}

void UNavigationButtonsContainer::OnLeftButtonClicked()
{
	if ( ActiveTab )
	{
		const int Index = NavigationButtonsBox->GetChildIndex(ActiveTab);
		if ( Index - 1 >= 0 )
		{
			ActiveTab = static_cast<UNavigationButtonWidget*>(NavigationButtonsBox->GetChildAt(Index - 1));
			ActiveTab->OnNavigationButtonClicked();
			SetNormalStateForOtherTabs();
		}
	}
}

void UNavigationButtonsContainer::OnRightButtonClicked()
{
	if ( ActiveTab )
	{
		const int Index = NavigationButtonsBox->GetChildIndex(ActiveTab);
		if ( Index + 1 < NavigationButtonsBox->GetChildrenCount() )
		{
			ActiveTab = static_cast<UNavigationButtonWidget*>(NavigationButtonsBox->GetChildAt(Index + 1));
			ActiveTab->OnNavigationButtonClicked();
			SetNormalStateForOtherTabs();
		}
	}
}

void UNavigationButtonsContainer::SetNormalStateForOtherTabs()
{
	for ( int i = 0; i < NavigationButtonsBox->GetChildrenCount(); ++i )
	{
		if ( NavigationButtonsBox->GetChildAt(i) != ActiveTab )
		{
			static_cast<UNavigationButtonWidget*>(NavigationButtonsBox->GetChildAt(i))->SetHoverState(false);
		}
	}
}

void UNavigationButtonsContainer::HandleImageVisibility(const bool bVisible)
{
	if (  !bVisible )
	{
		LeftButtonImage->SetVisibility(ESlateVisibility::Collapsed);
		RightButtonImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		LeftButtonImage->SetVisibility(ESlateVisibility::Visible);
		RightButtonImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UNavigationButtonsContainer::OnInputMethodChanged(const ECommonInputType CurrentInputType)
{
	if (  CurrentInputType == ECommonInputType::MouseAndKeyboard )
	{
		HandleImageVisibility(false);
	}
	else if ( CurrentInputType == ECommonInputType::Gamepad )
	{
		HandleImageVisibility(true);
	}
	
	ULocalSettings::Get()->CurrentInputType = CurrentInputType;
	
	Super::OnInputMethodChanged(CurrentInputType);
}

void UNavigationButtonsContainer::NativeConstruct()
{
	Super::NativeConstruct();

	if ( NavigationButtonsBox->GetChildrenCount() == 0 )
	{
		return;
	}
	
	ActiveTab = static_cast<UNavigationButtonWidget*>(NavigationButtonsBox->GetChildAt(0));
	
	if ( ULocalSettings::Get()->CurrentInputType == ECommonInputType::MouseAndKeyboard )
	{
		HandleImageVisibility(false);
	}
	else if ( ULocalSettings::Get()->CurrentInputType == ECommonInputType::Gamepad )
	{
		HandleImageVisibility(true);
		ActiveTab->SetHoverState(true);
	}
}