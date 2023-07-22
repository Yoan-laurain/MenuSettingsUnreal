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
	
	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChooseAKeyWidgetClass;

	void Refresh();
	virtual void UpdateHUD() override;
	
private :

	UPROPERTY()
	UChooseAKeyWidget* ChooseAKeyWidget;
	
protected :
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnTryKeyChange();
};