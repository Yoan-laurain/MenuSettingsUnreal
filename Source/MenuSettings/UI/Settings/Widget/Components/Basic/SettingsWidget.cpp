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

	if ( HoverButton )
	{
		HoverButton->OnHovered.AddDynamic(this, &USettingsWidget::OnHovered);
	}
}

void USettingsWidget::OnHovered()
{
	if (ParentWidget)
	{
		ParentWidget->ChangeDescription(SettingsItem->GetDescriptionRichText());
	}
}

void USettingsWidget::InitWidget( UGameSettingsItem* NewSettingsItem )
{
	Super::InitWidget(NewSettingsItem);
	SettingsItem->SetWidget(this);
	SetStateButtons();
}

void USettingsWidget::SetStateButtons()
{
	DecreaseButton->SetIsEnabled( !SettingsItem->IsMinValue(CurrentValue->GetText() ) );
	IncreaseButton->SetIsEnabled( !SettingsItem->IsMaxValue(CurrentValue->GetText() ) );
}

void USettingsWidget::UpdateParentOption()
{
	if ( SettingsItem->GetParentOption())
	{
		SettingsItem->GetParentOption()->SetIndexCurrentOption(SettingsItem->GetParentOption()->GetOptions().Num() - 1);
		SettingsItem->GetParentOption()->GetWidget()->SetCurrentValue( SettingsItem->GetParentOption()->GetCurrentOption() );
		SettingsItem->GetParentOption()->GetWidget()->SetStateButtons();
	}
}

void USettingsWidget::UpdateChildOption()
{
	for (const auto& ChildOption : SettingsItem->GetDependentOptions())
	{
		ensure(ChildOption);
		int Index = SettingsItem->GetIndexCurrentOption();
		ChildOption->SetIndexCurrentOption(Index);
		ChildOption->GetWidget()->SetCurrentValue( ChildOption->GetCurrentOption() );
		ChildOption->GetWidget()->SetStateButtons();
	}
}

void USettingsWidget::UpdateHUD()
{
	SetCurrentValue( SettingsItem->GetCurrentOption() );
	
	SetStateButtons();
	
	UpdateParentOption();

	if ( !SettingsItem->GetCurrentOption().ToString().Equals("Custom") )
	{
		UpdateChildOption();
	}
}

void USettingsWidget::OnDecreaseButtonClicked()
{
	SettingsItem->SetPreviousTechnicalOption<int>();
	UpdateHUD();
	ApplySetting();
}

void USettingsWidget::OnIncreaseButtonClicked()
{
	SettingsItem->SetNextTechnicalOption<int>();
	UpdateHUD();
	ApplySetting();
}