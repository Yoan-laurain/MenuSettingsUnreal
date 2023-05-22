#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SettingsWidget.generated.h"

class UTextBlock;
class UGameSettingsItem;
class UImage;
class FText;

UCLASS()
class MENUSETTINGS_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public :

	/** The name of the option */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SettingsText;
	
	UPROPERTY(meta = (BindWidget))
	UButton* DecreaseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* IncreaseButton;

	UPROPERTY( meta = (BindWidget) )
	UTextBlock* CurrentValue;
	
	void SetSettingsText(FText NewText);
	void SetCurrentValue(FText NewText);
	void InitWidget(UGameSettingsItem* SettingsItem);
	void ApplySettings();

private :

	UPROPERTY()
	UGameSettingsItem* SettingsItem;

protected :
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnDecreaseButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnIncreaseButtonClicked();
	
	virtual void NativeOnInitialized() override;
};