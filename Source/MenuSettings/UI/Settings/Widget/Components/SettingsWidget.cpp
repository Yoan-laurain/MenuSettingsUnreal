#include "SettingsWidget.h"
#include "Components/TextBlock.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

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
	SetSettingsText(SettingsItem->GetOptionName());
	SetCurrentValue( SettingsItem->GetDefaultOption() );
	DecreaseButton->SetIsEnabled(false);
}

void USettingsWidget::ApplySettings()
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->GetLocalPlayer()->GetLocalSettings()->ApplySettings(false);
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

	// SettingsItem->SetCurrentValue( SettingsItem->GetCurrentValue() - SettingsItem->GetOptionSourceStep() );
	
	// SettingsItem->SetOptionValue( SettingsItem->GetOptionValue() - SettingsItem->GetOptionSourceStep() );

	ApplySettings();
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
	// SettingsItem->SetOptionValue( SettingsItem->GetOptionValue() + SettingsItem->GetOptionSourceStep() );
	
	ApplySettings();
}