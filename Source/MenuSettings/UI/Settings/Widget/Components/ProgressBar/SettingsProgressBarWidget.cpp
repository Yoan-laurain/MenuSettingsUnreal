#include "SettingsProgressBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"

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
	ProgressBar->SetPercent(SettingsItem->GetTechnicalOption() / 100.0f);
}

void USettingsProgressBarWidget::OnValueChanged(float Value)
{
	ProgressBar->SetPercent(Value);
	SetCurrentValue( FText::FromString(FString::FromInt(static_cast<int>(Value * 100))) );
	SettingsItem->SetIndexCurrentOption(static_cast<int>(Value * 100));
	ApplySetting();
}
