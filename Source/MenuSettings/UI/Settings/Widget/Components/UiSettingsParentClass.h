#pragma once

#include "Blueprint/UserWidget.h"
#include "UiSettingsParentClass.generated.h"

class UTextBlock;
class UGameSettingsItem;
class FText;

UCLASS()
class UiSettingsParentClass : public UUserWidget
{

	GENERATED_BODY()
	
public:

	/** The name of the option */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SettingsText;

	UPROPERTY( meta = (BindWidget) )
	UTextBlock* CurrentValue;

	void SetSettingsText(FText NewText);
	void SetCurrentValue(FText NewText);
	virtual void InitWidget(UGameSettingsItem* SettingsItem);
	void ApplySetting();
	void SetParentWidget(class UMenuSettingsWidget* NewParentWidget);
	virtual void SetStateButtons();
	virtual void UpdateHUD();

protected :

	UPROPERTY()
	UGameSettingsItem* SettingsItem;
	
	UPROPERTY()
	UMenuSettingsWidget* ParentWidget;
};