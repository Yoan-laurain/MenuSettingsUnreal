#pragma once

#include "../UiSettingsParentClass.h"
#include "Components/Button.h"
#include "SettingsWidget.generated.h"

class UHorizontalBox;
class UGameSettingsItem;
class UImage;
class FText;

UCLASS()
class MENUSETTINGS_API USettingsWidget : public UiSettingsParentClass
{
	GENERATED_BODY()

public :
	
	UPROPERTY(meta = (BindWidget))
	UButton* DecreaseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* IncreaseButton;
	
	UPROPERTY( meta = (BindWidget) )
	UHorizontalBox* HorizontalBox;

	UPROPERTY( meta = (BindWidget) )
	UButton* HoverButton;

	virtual void InitWidget(UGameSettingsItem* SettingsItem) override;
	void SetStateButtons();
	void UpdateParentOption();
	void UpdateChildOption();
	void UpdateHUD();

protected :
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnDecreaseButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnIncreaseButtonClicked();
	
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnHovered();
};