#pragma once

#include "Blueprint/UserWidget.h"
#include "../../../../Foundation/CustomActivatableWidget.h"
#include "ValidationPopUpWidget.generated.h"

class UMenuSettingsWidget;
class UButton;
class UTextBlock;

UCLASS()
class MENUSETTINGS_API UValidationPopUpWidget final : public UCommonUserWidget
{
	GENERATED_BODY()

public :
	
	void SetMenuSettingsWidget(UMenuSettingsWidget* InMenuSettingsWidget);
	void SetTitleText(const FText& InTitleText);
	
	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetPrimaryGamepadFocusWidget();

	void SetShouldCloseMenuSettingsWidget(const bool bInShouldCloseMenuSettingsWidget);

private :

	UPROPERTY()
	UMenuSettingsWidget* MenuSettingsWidget;

	bool bShouldCloseMenuSettingsWidget;

#pragma region WidgetComponents
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

#pragma endregion WidgetComponents

protected :
	
	UFUNCTION(BlueprintCallable)
	void OnValidateButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnCancelButtonClicked();
	
	void HandleButtonClick(bool bIsValidateButton);
};