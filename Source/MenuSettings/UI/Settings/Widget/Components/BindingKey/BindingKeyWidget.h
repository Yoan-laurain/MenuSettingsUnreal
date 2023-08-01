#pragma once

#include "Blueprint/UserWidget.h"
#include "../UiSettingsParentClass.h"
#include "BindingKeyWidget.generated.h"

class UChooseAKeyWidget;
class UTextBlock;
class UButtonBase;

UCLASS()
class MENUSETTINGS_API UBindingKeyWidget final : public UiSettingsParentClass
{
	GENERATED_BODY()

public :

	// UiSettingsParentClass implementation Begin
	virtual void UpdateHUD() override;
	// UiSettingsParentClass implementation End

	virtual void SetCurrentValue(FText NewText) override;

#pragma region WidgetComponents
	
	UPROPERTY(meta = (BindWidget))
	UButtonBase* Button;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChooseAKeyWidgetClass;

#pragma endregion WidgetComponents

	void Refresh();
	
private :

	UPROPERTY()
	UChooseAKeyWidget* ChooseAKeyWidget;
	
protected :
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnTryKeyChange();
};