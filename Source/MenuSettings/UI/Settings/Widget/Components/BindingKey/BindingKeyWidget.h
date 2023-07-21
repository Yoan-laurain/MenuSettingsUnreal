#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuSettings/UI/Settings/Widget/Components/UiSettingsParentClass.h"
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
	
	void ClearKeyName();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChooseAKeyWidgetClass;
	
private :

	UPROPERTY()
	UChooseAKeyWidget* ChooseAKeyWidget;
	
protected :
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnTryKeyChange();
};