#pragma once

#include "Blueprint/UserWidget.h"
#include "ChooseAKeyWidget.generated.h"

UCLASS()
class MENUSETTINGS_API UChooseAKeyWidget final : public UUserWidget
{
	GENERATED_BODY()

public :
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Message;

	void HandleKeyChange(const FKey& Key);

private :

	TArray<FText> MessagesToDisplay;
	int IndexCurrentMessage = 0;
	
    virtual void NativeConstruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};