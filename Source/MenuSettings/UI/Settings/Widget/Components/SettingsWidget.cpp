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
	SetCurrentValue( SettingsItem->GetDefaultOption() );
	DecreaseButton->SetIsEnabled(false);
}

void USettingsWidget::OnDecreaseButtonClicked()
{
	SetCurrentValue( SettingsItem->GetPreviousOptions(CurrentValue->GetText() ) );
	
	SettingsItem->DecreaseCurrentValue();
	
	DecreaseButton->SetIsEnabled( SettingsItem->IsMinValue(CurrentValue->GetText() ) );

	IncreaseButton->SetIsEnabled(true);
}

void USettingsWidget::OnIncreaseButtonClicked()
{
	SetCurrentValue( SettingsItem->GetNextOptions(CurrentValue->GetText() ) );
	
	SettingsItem->IncreaseCurrentValue();
	
	IncreaseButton->SetIsEnabled( SettingsItem->IsMaxValue(CurrentValue->GetText() ) );
	
	DecreaseButton->SetIsEnabled(true);
}