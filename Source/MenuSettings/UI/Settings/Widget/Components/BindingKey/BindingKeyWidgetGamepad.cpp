#include "BindingKeyWidgetGamepad.h"
#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Image.h"
#include "MenuSettings/UI/Settings/Category/Bindings/Configuration/BindingConfiguration.h"
#include "MenuSettings/UI/Settings/Widget/MenuSettingsWidget.h"

void UBindingKeyWidgetGamepad::Refresh()
{
	UBindingConfiguration* Item = Cast<UBindingConfiguration>(GetSettingsItem());
	SetCurrentValue(Item->GetFirstMappableOption().GetKey());
}

void UBindingKeyWidgetGamepad::SetCurrentValue(const FKey& Key)
{
	const FSlateBrush SlateBrush = GetIconWithName(Key);

	if ( SlateBrush.GetResourceName().ToString().IsEmpty() )
	{
		IconImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::Visible);
		IconImage->SetBrush(SlateBrush);
	}

	if ( ParentWidget )
	{
		ParentWidget->OnSettingsDirtyStateChanged_Implementation(true);
	}
}

FSlateBrush UBindingKeyWidgetGamepad::GetIconWithName(const FKey& Key) const
{
	if (Key.IsValid()) 
	{
		const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
		
		FSlateBrush SlateBrush;

		ECommonInputType InputType = ECommonInputType::Gamepad;

		if (UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, Key, InputType, CommonInputSubsystem->GetCurrentGamepadName()))
		{
			return SlateBrush;
		}
		
		return FSlateBrush();
	}
	
	return FSlateBrush();
}

UEnhancedInputLocalPlayerSubsystem* UBindingKeyWidgetGamepad::GetEnhancedInputSubsystem() const
{
	const ULocalPlayer* BindingOwner = GetOwningLocalPlayer();
	
	return BindingOwner->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}