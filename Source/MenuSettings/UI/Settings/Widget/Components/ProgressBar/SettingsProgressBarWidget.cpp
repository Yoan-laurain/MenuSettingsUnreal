#include "SettingsProgressBarWidget.h"
#include "Components/ProgressBar.h"
#include "AnalogSlider.h"
#include "../../../Category/GameSettingsCollection.h"

void USettingsProgressBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if ( Slider )
	{
		Slider->OnValueChanged.AddDynamic(this, &USettingsProgressBarWidget::OnValueChanged);
	}
}

void USettingsProgressBarWidget::InitWidget(UGameSettingsItem* NewSettingsItem)
{
	Super::InitWidget(NewSettingsItem);
	UpdateHUD();
}

void USettingsProgressBarWidget::UpdateHUD()
{
	Super::UpdateHUD();
	ProgressBar->SetPercent(SettingsItem->GetTechnicalOption() / 100.0f);
	Slider->SetValue(SettingsItem->GetTechnicalOption() / 100.0f);
	SetCurrentValue( FText::FromString(FString::FromInt(SettingsItem->GetTechnicalOption())) );
}

void USettingsProgressBarWidget::OnValueChanged(float Value)
{
	ProgressBar->SetPercent(Value);
	SetCurrentValue( FText::FromString(FString::FromInt(static_cast<int>(Value * 100))) );
	SettingsItem->SetIndexCurrentOption(static_cast<int>(Value * 100));
	ApplySetting();
}