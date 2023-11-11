#include "ChooseAKeyWidget.h"
#include "BindingKeyWidget.h"
#include "CommonInputTypeEnum.h"
#include "Components/TextBlock.h"
#include "Input/Events.h"
#include "../../../LocalSettings.h"
#include "../../../Category/Bindings/Configuration/BindingConfiguration.h"
#include "../../MenuSettingsWidget.h"

#define LOCTEXT_NAMESPACE "MySettings"

void UChooseAKeyWidget::HandleKeySelection(const FKey& Key)
{
	const FText MessageText = FText::Format(LOCTEXT("ConfirmBinding",
	                      "The key {0} is already used for another action are you sure you want to bind it? Press {1} to confirm or esc to cancel"),
	                      FText::FromString(Key.ToString()),
	                      FText::FromString(EKeys::Enter.ToString()));
	
	Message->SetText( MessageText );

	CurrentKey = Key;
}

void UChooseAKeyWidget::HandleKeyChange(const FKey& Key)
{
	if (Key.IsValid())
	{
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
		//Settings->GetAllBindingConfigurationsFromKey(0,Key , OutBindingConfiguration);
	}
	
	if (!OutBindingConfiguration.IsEmpty())
	{
		for (UBindingConfiguration* BindingConfiguration : OutBindingConfiguration)
		{
			BindingConfiguration->ClearBindings();

			UBindingKeyWidget* BindingKeyWidget = Cast<UBindingKeyWidget>(BindingConfiguration->GetWidget());
			BindingKeyWidget->Refresh();
		}
	}
	
	Item->RegisterNewBinding(Key);
	
	const FText KeyName = FText::FromString(Key.ToString());
	ensure(Parent);
	ensure(Parent->GetParentWidget());
	Parent->GetParentWidget()->SetPendingModificationState(true);
	
	Parent->Refresh();
	Parent->SetInternalFocus();
	RemoveFromParent();
}

void UChooseAKeyWidget::SetTypeInputExpected(ECommonInputType InTypeInputExpected)
{
	ExpectedInputType = InTypeInputExpected;
}

bool UChooseAKeyWidget::ExitHandle(const FKey& Key)
{
	// if its esc or B button
	if ( Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right)
	{
		Parent->SetInternalFocus();
		RemoveFromParent();
		return true;
	}
	return false;
}

void UChooseAKeyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if ( ULocalSettings::Get()->CurrentInputType == ECommonInputType::MouseAndKeyboard)
	{
		MessagesToDisplay.Add(LOCTEXT("ChooseAKeyWidget_Title", "Press a key to bind or esc to cancel"));
	}
	else
	{
		MessagesToDisplay.Add(LOCTEXT("ChooseAKeyWidget_TitleGamepad", "Press a button to bind or start to cancel"));
	}

	Message->SetText( MessagesToDisplay[0] );
	SetIsFocusable(true);

	if ( Parent && Parent->GetParentWidget() )
	{
		Parent->GetParentWidget()->DeactivateWidget();
	}
}

void UChooseAKeyWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if ( Parent && Parent->GetParentWidget() )
	{
		Parent->GetParentWidget()->ActivateWidget();
	}
}

FReply UChooseAKeyWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply =  Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if ( !ExitHandle(InKeyEvent.GetKey() ) )
	{
		if (ULocalSettings::Get()->CurrentInputType == ExpectedInputType )
		{
			HandleKeyChange(InKeyEvent.GetKey());
		}
	}
	
	return Reply;
}

FReply UChooseAKeyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (ULocalSettings::Get()->CurrentInputType != ECommonInputType::Gamepad )
	{
		HandleKeyChange(InMouseEvent.GetEffectingButton());
	}
	
	return Reply;
}