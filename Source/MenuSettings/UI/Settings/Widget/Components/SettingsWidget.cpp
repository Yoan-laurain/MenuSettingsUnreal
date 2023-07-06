#include "SettingsWidget.h"
#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"

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

void USettingsWidget::SetSettingsText(const FText NewText)
{
	if ( SettingsText )
	{
		SettingsText->SetText(NewText);
	}
}

void USettingsWidget::SetCurrentValue(const FText NewText)
{
	if ( CurrentValue )
	{
		CurrentValue->SetText(NewText);
	}
}

void USettingsWidget::InitWidget( UGameSettingsItem* NewSettingsItem )
{
	SettingsItem = NewSettingsItem;
	SetSettingsText(SettingsItem->GetOptionName());
	SetCurrentValue( SettingsItem->GetCurrentOption() ); 
	SetStateButtons();
}

void USettingsWidget::SetStateButtons()
{
	DecreaseButton->SetIsEnabled( !SettingsItem->IsMinValue(CurrentValue->GetText() ) );
	IncreaseButton->SetIsEnabled( !SettingsItem->IsMaxValue(CurrentValue->GetText() ) );
}

void USettingsWidget::OnDecreaseButtonClicked()
{
	// Change current value
	SetCurrentValue( SettingsItem->GetPreviousOptions(CurrentValue->GetText() ) );

	// Change the real settings value
	SettingsItem->SetPreviousTechnicalOption<int>();
	
	// enable/disable buttons
	SetStateButtons();
}

void USettingsWidget::OnIncreaseButtonClicked()
{
	SetCurrentValue( SettingsItem->GetNextOptions(CurrentValue->GetText() ) );
	
	SettingsItem->SetNextTechnicalOption<int>();
	
	SetStateButtons();
}