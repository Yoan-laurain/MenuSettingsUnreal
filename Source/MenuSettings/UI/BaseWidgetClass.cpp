#include "BaseWidgetClass.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"

FReply UBaseWidgetClass::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem();
	UE_LOG(LogTemp, Warning, TEXT("InputSubsystem: %s"), InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad ? TEXT("Gamepad") : TEXT("Keyboard"));
	if (InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* PrimaryFocus = GetPrimaryGamepadFocusWidget())
		{
			TSharedPtr<SWidget> WidgetToFocus = PrimaryFocus->GetCachedWidget();
			if (WidgetToFocus.IsValid())
			{
				return FReply::Handled().SetUserFocus(WidgetToFocus.ToSharedRef(), InFocusEvent.GetCause());
			}
		}
	}

	return FReply::Unhandled();
}