#include "SettingsWidget.h"
#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/GameSettingsCollection.h"

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

void USettingsWidget::SetSettingsText(FText NewText)
{
	if ( SettingsText )
	{
		SettingsText->SetText(NewText);
	}
}

void USettingsWidget::SetCurrentValue(FText NewText)
{
	if ( CurrentValue )
	{
		CurrentValue->SetText(NewText);
	}
}

void USettingsWidget::InitWidget( UGameSettingsItem* NewSettingsItem )
{
	SettingsItem = NewSettingsItem;
	SetSettingsText(SettingsItem->GetNavigationText());
	SetCurrentValue( SettingsItem->GetDefaultOption() );
	DecreaseButton->SetIsEnabled(false);
}

void USettingsWidget::OnDecreaseButtonClicked()
{
	SetCurrentValue( SettingsItem->GetPreviousOptions(CurrentValue->GetText() ) );

	if ( SettingsItem->IsMinValue(CurrentValue->GetText() ) )
	{
		DecreaseButton->SetIsEnabled(false);
	}
	else
	{
		DecreaseButton->SetIsEnabled(true);
	}

	IncreaseButton->SetIsEnabled(true);
}

void USettingsWidget::OnIncreaseButtonClicked()
{
	SetCurrentValue( SettingsItem->GetNextOptions(CurrentValue->GetText() ) );

	if ( SettingsItem->IsMaxValue(CurrentValue->GetText() ) )
	{
		IncreaseButton->SetIsEnabled(false);
	}
	else
	{
		IncreaseButton->SetIsEnabled(true);
	}

	DecreaseButton->SetIsEnabled(true);
}