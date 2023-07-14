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
	
	UPROPERTY(meta = (BindWidget))
	UButton* ValidateButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

	void SetMenuSettingsWidget(UMenuSettingsWidget* InMenuSettingsWidget);

private :

	UPROPERTY()
	UMenuSettingsWidget* MenuSettingsWidget;

protected :
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void OnValidateButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();
};