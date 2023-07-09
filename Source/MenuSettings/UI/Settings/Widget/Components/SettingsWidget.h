#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SettingsWidget.generated.h"

class UHorizontalBox;
class UTextBlock;
class UGameSettingsItem;
class UImage;
class FText;

UCLASS()
class MENUSETTINGS_API USettingsWidget final : public UUserWidget
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

	UPROPERTY( meta = (BindWidget) )
	UHorizontalBox* HorizontalBox;

	UPROPERTY( meta = (BindWidget) )
	UButton* HoverButton;
	
	void SetSettingsText(FText NewText);
	void SetCurrentValue(FText NewText);
	void InitWidget(UGameSettingsItem* SettingsItem);
	void SetStateButtons();
	void UpdateParentOption();
	void UpdateChildOption();
	void UpdateHUD();
	void SetParentWidget(class UMenuSettingsWidget* NewParentWidget);

private :

	UPROPERTY()
	UGameSettingsItem* SettingsItem;

	UPROPERTY()
	UMenuSettingsWidget* ParentWidget;

protected :
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnDecreaseButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnIncreaseButtonClicked();
	
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnHovered();
};