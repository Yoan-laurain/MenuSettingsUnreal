#include "ChooseAKeyWidget.h"
#include "BindingKeyWidget.h"
#include "Components/TextBlock.h"
#include "Input/Events.h"
#include "../../../LocalSettings.h"
#include "../../../Category/Bindings/Configuration/BindingConfiguration.h"
#include "MenuSettings/UI/Settings/Widget/MenuSettingsWidget.h"

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
		Settings->GetAllBindingConfigurationsFromKey(0,Key , OutBindingConfiguration);
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

	// prevent from triggering ( bottom action ) when binding with gamepad
	if ( Key == EKeys::Gamepad_FaceButton_Right || Key == EKeys::Gamepad_FaceButton_Top || Key == EKeys::Gamepad_FaceButton_Left ) 
	{
		Parent->GetParentWidget()->SetHasBindSpecialAction(true);
	}
	
	Parent->Refresh();
	Parent->SetInternalFocus();
	Parent->GetParentWidget()->SetCanUseBottomActions(true);
	RemoveFromParent();
}

void UChooseAKeyWidget::SetTypeInputExpected(ECommonInputType InTypeInputExpected)
{
	ExpectedInputType = InTypeInputExpected;
}

void UChooseAKeyWidget::ExitHandle(const FKey& Key)
{
	// if its esc or B button
	if ( Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right)
	{
		Parent->SetInternalFocus();
		Parent->GetParentWidget()->SetCanUseBottomActions(true);
		RemoveFromParent();
		return;
	}
}

UWidget* UChooseAKeyWidget::NativeGetDesiredFocusTarget() const
{
	UE_LOG(LogTemp, Warning, TEXT("NativeGetDesiredFocusTarget"));
	return const_cast<UChooseAKeyWidget*>(this);
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
}

FReply UChooseAKeyWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply =  Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	ExitHandle(InKeyEvent.GetKey());
	
	if (ULocalSettings::Get()->CurrentInputType == ExpectedInputType )
	{
		UE_LOG(LogTemp, Warning, TEXT("Key pressed : %s"), *InKeyEvent.GetKey().ToString());
		HandleKeyChange(InKeyEvent.GetKey());
	}
	
	return Reply;
}

FReply UChooseAKeyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (ULocalSettings::Get()->CurrentInputType == ExpectedInputType )
	{
		UE_LOG(LogTemp, Warning, TEXT("Key pressed : %s"), *InMouseEvent.GetEffectingButton().ToString());
		HandleKeyChange(InMouseEvent.GetEffectingButton());
	}
	
	return Reply;
}