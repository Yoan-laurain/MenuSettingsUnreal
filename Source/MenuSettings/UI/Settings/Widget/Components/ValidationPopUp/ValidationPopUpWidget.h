#pragma once

#include "Blueprint/UserWidget.h"
#include "ValidationPopUpWidget.generated.h"

class UMenuSettingsWidget;
class UButton;
class UTextBlock;

UCLASS()
class MENUSETTINGS_API UValidationPopUpWidget final : public UUserWidget
{
	GENERATED_BODY()

public :

#pragma region WidgetComponents
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

#pragma endregion WidgetComponents

	void SetMenuSettingsWidget(UMenuSettingsWidget* InMenuSettingsWidget);

private :

	UPROPERTY()
	UMenuSettingsWidget* MenuSettingsWidget;

protected :
	
	UFUNCTION(BlueprintCallable)
	void OnValidateButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnCancelButtonClicked();
};