#pragma once

#include "../Basic/SettingsWidget.h"
#include "SettingsProgressBarWidget.generated.h"

class UProgressBar;

UCLASS()
class MENUSETTINGS_API USettingsProgressBarWidget final : public UiSettingsParentClass
{
	GENERATED_BODY()

public :
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	class USlider* Slider;

	virtual void InitWidget(UGameSettingsItem* NewSettingsItem) override;
	virtual void UpdateHUD() override;

protected:
	
	UFUNCTION()
	void OnValueChanged(float Value);
	
	virtual void NativeOnInitialized() override;
};