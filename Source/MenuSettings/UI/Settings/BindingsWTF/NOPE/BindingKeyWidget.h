#pragma once

#include "Blueprint/UserWidget.h"
#include "BindingKeyWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class MENUSETTINGS_API UBindingKeyWidget final : public UUserWidget
{
	GENERATED_BODY()

public :
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActionName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KeyName;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	void InitWidget(const FText NewActionName, const FText NewKeyName);
	void ClearKeyName();
	
protected :
	virtual void NativeOnInitialized() override;
	void OnTryKeyChange();
};