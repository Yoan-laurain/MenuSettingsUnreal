#pragma once

#include "Blueprint/UserWidget.h"
#include "../UiSettingsParentClass.h"
#include "BindingKeyWidget.generated.h"

class UChooseAKeyWidget;
class UTextBlock;
class UButton;

UCLASS()
class MENUSETTINGS_API UBindingKeyWidget final : public UiSettingsParentClass
{
	GENERATED_BODY()

public :

	// UiSettingsParentClass implementation Begin
	virtual void UpdateHUD() override;
	// UiSettingsParentClass implementation End

#pragma region WidgetComponents
	
	UPROPERTY(meta = (BindWidget))
	UButton* Button;

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