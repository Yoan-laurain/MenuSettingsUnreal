#pragma once

#include "../UiSettingsParentClass.h"
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

#pragma region WidgetComponents
	
	UPROPERTY(meta = (BindWidget))
	UButton* DecreaseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* IncreaseButton;
	
	UPROPERTY( meta = (BindWidget) )
	UHorizontalBox* HorizontalBox;

#pragma endregion WidgetComponents

	virtual void InitWidget(UGameSettingsItem* SettingsItem) override;
	
	void SetStateButtons();
	
	void UpdateParentOption();
	void UpdateChildOption(const bool SetLikeParent);
	
	void UpdateParentHUD();
	void UpdateChildHUD();
	
	void UpdateHUD();
	void ApplySettingsAndUpdateUI();

protected :
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnDecreaseButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnIncreaseButtonClicked();
	
	virtual void NativeOnInitialized() override;
};