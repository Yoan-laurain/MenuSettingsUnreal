#pragma once

#include "Blueprint/UserWidget.h"
#include "../UiSettingsParentClass.h"
#include "ChooseAKeyWidget.generated.h"

class UBindingKeyWidget;
UCLASS()
class MENUSETTINGS_API UChooseAKeyWidget final : public UUserWidget
{
	GENERATED_BODY()

public :
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Message;
	
	void HandleKeySelection(const FKey& Key);
	void HandleKeyChange(const FKey& Key);
	void SetParent(UBindingKeyWidget* InParent);
	TArray<FText> GetMessagesToDisplay() const { return MessagesToDisplay; }
	void ValidateKey(const FKey& Key);

private :

	FKey CurrentKey;

	UPROPERTY()
	UBindingKeyWidget* Parent = nullptr;
	
	TArray<FText> MessagesToDisplay;
	int IndexCurrentMessage = 0;
	
    virtual void NativeConstruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};