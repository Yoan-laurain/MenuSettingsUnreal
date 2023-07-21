#include "MenuSettings/UI/Settings/Widget/Components/BindingKey/ChooseAKeyWidget.h"
#include "Input/Events.h"
#include "MenuSettings/UI/Settings/InputDataAsset.h"
#include "MenuSettings/UI/Settings/Category/Mouse&Keyboard/AssetManager/AssetManagerCustom.h"
#include "MenuSettings/UI/Settings/Category/Mouse&Keyboard/Configuration/MappableConfigPair.h"

void UChooseAKeyWidget::HandleKeyChange(const FKey& Key)
{
	if (Key.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *Key.ToString());
		
		
		
	}
}

void UChooseAKeyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MessagesToDisplay.Add(FText::FromString("Press a key to bind"));
	MessagesToDisplay.Add(FText::FromString("The key %s is already used for another action are you sure you want to bind it? Press %s to confirm"));
}

FReply UChooseAKeyWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply =  Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	HandleKeyChange(InKeyEvent.GetKey());
	
	return Reply;
}

FReply UChooseAKeyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	HandleKeyChange(InMouseEvent.GetEffectingButton());
	
	return Reply;
}