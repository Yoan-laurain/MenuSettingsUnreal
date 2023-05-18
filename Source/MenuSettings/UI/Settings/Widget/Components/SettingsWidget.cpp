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

void USettingsWidget::SetImage(unsigned IndexImage)
{
	for (const auto& Elem : ImageMap)
	{
		if ( Elem.Key == IndexImage )
		{
			Image = Elem.Value;
			CurrentImageIndex = IndexImage;
			break;
		}
	} 
}

void USettingsWidget::InitWidget(const UGameSettingsItem* SettingsItem)
{
	CurrentImageIndex = 0;
	SetImage(0);
	SetSettingsText(SettingsItem->GetNavigationText());
}

void USettingsWidget::OnDecreaseButtonClicked()
{
	const unsigned NewIndex = CurrentImageIndex - 1;
	SetImage(NewIndex);
}

void USettingsWidget::OnIncreaseButtonClicked()
{
	const unsigned NewIndex = CurrentImageIndex + 1;
	SetImage(NewIndex);
}