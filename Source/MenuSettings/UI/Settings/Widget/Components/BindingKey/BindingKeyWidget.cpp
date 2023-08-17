#include "BindingKeyWidget.h"
#include "ChooseAKeyWidget.h"
#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "../../../../Components/ButtonBase.h"
#include "Components/Image.h"
#include "EnhancedInputSubsystems.h"
#include "MenuSettings/UI/Settings/Category/Bindings/Configuration/BindingConfiguration.h"
#include "MenuSettings/UI/Settings/Widget/MenuSettingsWidget.h"

void UBindingKeyWidget::Refresh()
{
	UBindingConfiguration* Item = Cast<UBindingConfiguration>(GetSettingsItem());
	SetCurrentValue(Item->GetFirstMappableOption().GetKey());

	// if we do not have icon for this key then set it by text
	if ( IconImage->GetVisibility() == ESlateVisibility::Hidden )
	{
		SetCurrentValue(Item->GetPrimaryKeyText());
	}
}

void UBindingKeyWidget::SetInternalFocus()
{
	SetIsEnabled(true);
	if ( Button )
	{
		Button->SetFocus();
	}
}

void UBindingKeyWidget::InitWidget(UGameSettingsItem* NewSettingsItem)
{
	SettingsItem = NewSettingsItem;
	SettingsItem->SetWidget(this);
	SetSettingsText(SettingsItem->GetOptionName());
	Refresh();
}

void UBindingKeyWidget::SetTypeInputExpected(ECommonInputType InTypeInputExpected)
{
	ExpectedInputType = InTypeInputExpected;
}

FSlateBrush UBindingKeyWidget::GetIconWithName(const FKey& Key) const
{
	if (Key.IsValid()) 
	{
		const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
		
		FSlateBrush SlateBrush;
		
		if (UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, Key, ExpectedInputType , CommonInputSubsystem->GetCurrentGamepadName()))
		{
			return SlateBrush;
		}
		
		return FSlateBrush();
	}
	
	return FSlateBrush();
}

UEnhancedInputLocalPlayerSubsystem* UBindingKeyWidget::GetEnhancedInputSubsystem() const
{
	const ULocalPlayer* BindingOwner = GetOwningLocalPlayer();
	
	return BindingOwner->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}

void UBindingKeyWidget::UpdateHUD()
{
	Refresh();
}

void UBindingKeyWidget::SetCurrentValue(const FText NewText)
{
	Button->SetButtonText(NewText);

	if ( ParentWidget )
	{
		ParentWidget->OnSettingsDirtyStateChanged_Implementation(true);
	}
}

void UBindingKeyWidget::SetCurrentValue(const FKey& Key)
{
	const FSlateBrush SlateBrush = GetIconWithName(Key);

	if ( !SlateBrush.GetResourceObject() )
	{
		IconImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		IconImage->SetBrush(SlateBrush);
		SetCurrentValue(FText::GetEmpty());
	}

	if ( ParentWidget )
	{
		ParentWidget->OnSettingsDirtyStateChanged_Implementation(true);
	}
}

void UBindingKeyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if ( Button )
	{
		Button->OnClicked().AddUObject(this, &UBindingKeyWidget::OnTryKeyChange);
	}
}

void UBindingKeyWidget::OnTryKeyChange()
{
	if ( ChooseAKeyWidgetClass )
	{
		ChooseAKeyWidget = CreateWidget<UChooseAKeyWidget>(GetWorld(), ChooseAKeyWidgetClass);
		
		if ( ChooseAKeyWidget )
		{
			ChooseAKeyWidget->SetParent(this);
			ChooseAKeyWidget->SetTypeInputExpected(ExpectedInputType);
			ChooseAKeyWidget->AddToViewport();
		}
	}
}