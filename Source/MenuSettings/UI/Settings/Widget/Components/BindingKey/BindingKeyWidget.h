#pragma once

#include "Blueprint/UserWidget.h"
#include "../UiSettingsParentClass.h"
#include "BindingKeyWidget.generated.h"

enum class ECommonInputType : uint8;
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
	
	UPROPERTY(meta = (BindWidget),EditAnywhere,BlueprintReadWrite)
	UButtonBase* Button;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChooseAKeyWidgetClass;

#pragma endregion WidgetComponents

	void Refresh();
	void SetInternalFocus();
	
	void SetTypeInputExpected(ECommonInputType InTypeInputExpected);
	
private :

	UPROPERTY()
	UChooseAKeyWidget* ChooseAKeyWidget;

	ECommonInputType ExpectedInputType;
	
protected :
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnTryKeyChange();
};