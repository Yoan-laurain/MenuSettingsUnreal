#pragma once

#include "CommonUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "UiSettingsParentClass.generated.h"

class UTextBlock;
class UGameSettingsItem;
class FText;
class UButton;

UCLASS()
class UiSettingsParentClass : public UCommonUserWidget
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
	virtual void SetCurrentValue(FText NewText);

	void SetParentWidget(class UMenuSettingsWidget* NewParentWidget);
	UMenuSettingsWidget* GetParentWidget() const { return ParentWidget; }
	
	UGameSettingsItem* GetSettingsItem() const;

#pragma endregion GettersSetters
	
	virtual void InitWidget(UGameSettingsItem* SettingsItem);
	
	void ApplySetting();

	virtual void SetStateButtons();
	
	virtual void UpdateHUD();

	UFUNCTION(BlueprintCallable)
	void OnHover();

#pragma region GAMEPAD

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetPrimaryGamepadFocusWidget();
	
#pragma endregion GAMEPAD
	
protected :

	UPROPERTY()
	UGameSettingsItem* SettingsItem;
	
	UPROPERTY()
	UMenuSettingsWidget* ParentWidget;
};