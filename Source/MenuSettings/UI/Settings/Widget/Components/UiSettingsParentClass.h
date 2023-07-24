#pragma once

#include "Blueprint/UserWidget.h"
#include "UiSettingsParentClass.generated.h"

class UTextBlock;
class UGameSettingsItem;
class FText;
class UButton;

UCLASS()
class UiSettingsParentClass : public UUserWidget
{

	GENERATED_BODY()
	
public:

#pragma region WidgetComponents
	
	/** The name of the option */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SettingsText;

	UPROPERTY( meta = (BindWidget) )
	UTextBlock* CurrentValue;

#pragma endregion WidgetComponents

#pragma region GettersSetters

	void SetSettingsText(FText NewText);
	void SetCurrentValue(FText NewText);

	void SetParentWidget(class UMenuSettingsWidget* NewParentWidget);
	
	UGameSettingsItem* GetSettingsItem() const;

#pragma endregion GettersSetters
	
	virtual void InitWidget(UGameSettingsItem* SettingsItem);
	
	void ApplySetting();

	virtual void SetStateButtons();
	
	virtual void UpdateHUD();

	UFUNCTION(BlueprintCallable)
	void OnHover();

protected :

	UPROPERTY()
	UGameSettingsItem* SettingsItem;
	
	UPROPERTY()
	UMenuSettingsWidget* ParentWidget;
};