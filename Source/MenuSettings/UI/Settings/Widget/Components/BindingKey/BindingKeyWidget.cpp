#include "BindingKeyWidget.h"
#include "ChooseAKeyWidget.h"
#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "../../../../Components/ButtonBase.h"
#include "Components/Image.h"
#include "EnhancedInputSubsystems.h"
#include "../../../../Settings/Widget/MenuSettingsWidget.h"
#include "MenuSettings/UI/Settings/Category/Bindings/CustomSettingKeyboardInput.h"

void UBindingKeyWidget::UnMapKey()
{
	UCustomSettingKeyboardInput* Item = Cast<UCustomSettingKeyboardInput>(GetSettingsItem());

	Item->ClearOptions();

	Refresh();
}

void UBindingKeyWidget::Refresh()
{
	UCustomSettingKeyboardInput* Item = Cast<UCustomSettingKeyboardInput>(GetSettingsItem());

	if ( Item->GetOptions().Num() == 0 )
	{
		SetCurrentValue(FText::GetEmpty());
		return;
	}

	// TODO : GetKeyTextFromSlot(EPlayerMappableKeySlot::First))
	FText KeyText = Item->GetOptions()[0];
	
	SetCurrentValue(KeyText);
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