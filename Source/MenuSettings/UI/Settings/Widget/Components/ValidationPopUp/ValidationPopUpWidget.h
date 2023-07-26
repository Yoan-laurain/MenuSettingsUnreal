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
	
	void SetMenuSettingsWidget(UMenuSettingsWidget* InMenuSettingsWidget);
	void SetTitleText(const FText& InTitleText);

private :

	UPROPERTY()
	UMenuSettingsWidget* MenuSettingsWidget;

#pragma region WidgetComponents
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

#pragma endregion WidgetComponents

protected :
	
	UFUNCTION(BlueprintCallable)
	void OnValidateButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnCancelButtonClicked();
};