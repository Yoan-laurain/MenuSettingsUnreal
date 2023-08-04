#pragma once

#include "Blueprint/UserWidget.h"
#include "ChooseAKeyWidget.generated.h"

enum class ECommonInputType : uint8;
class UBindingKeyWidget;
class UTextBlock;

UCLASS()
class MENUSETTINGS_API UChooseAKeyWidget final : public UUserWidget
{
	GENERATED_BODY()

public :
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Message;

#pragma region GettersSetters

	void SetParent(UBindingKeyWidget* InParent);
	TArray<FText> GetMessagesToDisplay() const { return MessagesToDisplay; }

#pragma endregion GettersSetters
	
	void HandleKeySelection(const FKey& Key);
	void HandleKeyChange(const FKey& Key);
	void ValidateKey(const FKey& Key);
	void SetTypeInputExpected(ECommonInputType InTypeInputExpected);
	void ExitHandle(const FKey& Key);

private :

	FKey CurrentKey;

	UPROPERTY()
	UBindingKeyWidget* Parent = nullptr;
	
	TArray<FText> MessagesToDisplay;
	
	int IndexCurrentMessage = 0;
	ECommonInputType ExpectedInputType;
	
    virtual void NativeConstruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};