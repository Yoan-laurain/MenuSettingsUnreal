#include "SettingsWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Widget/MenuSettingsWidget.h"

void USettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if ( DecreaseButton )
	{
		DecreaseButton->OnClicked.AddDynamic(this, &USettingsWidget::OnDecreaseButtonClicked);
	}

	if ( IncreaseButton )
	{
		IncreaseButton->OnClicked.AddDynamic(this, &USettingsWidget::OnIncreaseButtonClicked);
	}
}

void USettingsWidget::InitWidget( UGameSettingsItem* NewSettingsItem )
{
	Super::InitWidget(NewSettingsItem);
	SetStateButtons();
}

void USettingsWidget::SetStateButtons()
{
	DecreaseButton->SetIsEnabled( !SettingsItem->IsMinValue(CurrentValue->GetText() ) );
	IncreaseButton->SetIsEnabled( !SettingsItem->IsMaxValue(CurrentValue->GetText() ) );
}

void USettingsWidget::UpdateParentOption()
{
	for (const auto& ParentOption : SettingsItem->GetParentOptions())
	{
		ParentOption->SetIndexCurrentOption( ParentOption->GetParentUniqueOption() );
	}
}

void USettingsWidget::UpdateChildOption(const bool SetLikeParent)
{
	for (const auto& ChildOption : SettingsItem->GetDependentOptions())
	{
		if ( SetLikeParent )
		{
			ChildOption->SetIndexCurrentOption(SettingsItem->GetIndexCurrentOption());
		}
		else
		{
			ChildOption->SetIndexCurrentOption( ChildOption->GetIndexFromFile() );
		}
	}
}

void USettingsWidget::UpdateHUD()
{
	SetCurrentValue( SettingsItem->GetCurrentOption() );
	SetStateButtons();
	UpdateParentHUD();
	UpdateChildHUD();
}

void USettingsWidget::UpdateParentHUD()
{
	for (const auto& ParentOption : SettingsItem->GetParentOptions())
	{
		ParentOption->GetWidget()->SetCurrentValue( ParentOption->GetCurrentOption() );
		ParentOption->GetWidget()->SetStateButtons();
	}
}

void USettingsWidget::UpdateChildHUD()
{
	for (const auto& ChildOption : SettingsItem->GetDependentOptions())
	{
		ChildOption->GetWidget()->SetCurrentValue( ChildOption->GetCurrentOption() );
		ChildOption->GetWidget()->SetStateButtons();
	}
}

void USettingsWidget::ApplySettingsAndUpdateUI()
{
	UpdateParentOption();
	
	const bool SetLikeParent = !SettingsItem->GetCurrentOption().ToString().Equals("Off") && !SettingsItem->GetCurrentOption().ToString().Equals("On");

	if ( !SettingsItem->GetCurrentOption().ToString().Equals("Custom") && SetLikeParent )
	{
		UpdateChildOption(SetLikeParent);
	}
	
	ApplySetting();

	// in case of auto set quality we want to update the HUD after the apply setting
	if ( !SettingsItem->GetCurrentOption().ToString().Equals("Custom") && !SetLikeParent )
	{
		UpdateChildOption(SetLikeParent);
	}
	
	UpdateHUD();
}

void USettingsWidget::OnDecreaseButtonClicked()
{
	SettingsItem->SetPreviousTechnicalOption<int>();
	ApplySettingsAndUpdateUI();
}

void USettingsWidget::OnIncreaseButtonClicked()
{
	SettingsItem->SetNextTechnicalOption<int>();
	ApplySettingsAndUpdateUI();
}