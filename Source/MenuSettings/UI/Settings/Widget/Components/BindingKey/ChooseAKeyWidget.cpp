#include "MenuSettings/UI/Settings/Widget/Components/BindingKey/ChooseAKeyWidget.h"
#include "BindingKeyWidget.h"
#include "Components/TextBlock.h"
#include "Input/Events.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/Mouse&Keyboard/AssetManager/AssetManagerCustom.h"
#include "MenuSettings/UI/Settings/Category/Mouse&Keyboard/Configuration/BindingConfiguration.h"
#include "MenuSettings/UI/Settings/Category/Mouse&Keyboard/Configuration/MappableConfigPair.h"

void UChooseAKeyWidget::HandleKeySelection(const FKey& Key)
{
	const FText MessageText = FText::FromString(
		" The key " + FText::FromString(Key.ToString()).ToString() +
		" is already used for another action are you sure you want to bind it? Press " +
		FText::FromString(EKeys::Enter.ToString()).ToString() + " to confirm or esc to cancel");
	Message->SetText( MessageText );

	CurrentKey = Key;
}

void UChooseAKeyWidget::HandleKeyChange(const FKey& Key)
{
	if (Key.IsValid())
	{
		// if its esc
		if ( Key == EKeys::Escape)
		{
			RemoveFromParent();
			return;
		}

		// if its enter
		if ( Key == EKeys::Enter)
		{
			if ( CurrentKey.IsValid() )
			{
				ValidateKey(CurrentKey);
			}
			return;
		}
		
		if ( Message->GetText().EqualTo(FText::FromString(MessagesToDisplay[0].ToString())))
		{
			const UBindingConfiguration* Item = Cast<UBindingConfiguration>(Parent->GetSettingsItem());

			TArray<FName> MappedActions;
			Item->GetAllMappedActionsFromKey(0,Key,MappedActions);

			if (MappedActions.Num() > 0)
			{
				HandleKeySelection(Key);
			}
			else
			{
				ValidateKey(Key);
			}
		}
		else
		{
			if ( CurrentKey.IsValid() && Key == CurrentKey )
			{
				ValidateKey(CurrentKey);
			}
			else
			{
				HandleKeySelection(Key);
			}
		}
	}
}

void UChooseAKeyWidget::SetParent(UBindingKeyWidget* InParent)
{
	Parent = InParent;
}

void UChooseAKeyWidget::ValidateKey(const FKey& Key)
{
	UBindingConfiguration* Item = Cast<UBindingConfiguration>(Parent->GetSettingsItem());
	const ULocalSettings* Settings = ULocalSettings::Get();
	
	TArray<UBindingConfiguration*> OutBindingConfiguration;

	if (Settings)
	{
		Settings->GetAllBindingConfigurationsFromKey(0,Key , OutBindingConfiguration);
	}
	
	if (!OutBindingConfiguration.IsEmpty())
	{
		for (UBindingConfiguration* BindingConfiguration : OutBindingConfiguration)
		{
			BindingConfiguration->Clear();

			UBindingKeyWidget* BindingKeyWidget = Cast<UBindingKeyWidget>(BindingConfiguration->GetWidget());
			BindingKeyWidget->Refresh();
		}
	}
	
	Item->ChangeBinding(0, Key);

	const FText KeyName = FText::FromString(Key.ToString());
	Parent->Refresh();
	RemoveFromParent();
}

void UChooseAKeyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MessagesToDisplay.Add(FText::FromString("Press a key to bind or esc to cancel"));
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