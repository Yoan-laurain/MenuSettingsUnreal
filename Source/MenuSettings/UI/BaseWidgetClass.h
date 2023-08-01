#pragma once

#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BaseWidgetClass.generated.h"

UCLASS(Abstract, meta = (Category = "Settings", DisableNativeTick))
class MENUSETTINGS_API UBaseWidgetClass : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:

	// Focus transitioning to subwidgets for the gamepad
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetPrimaryGamepadFocusWidget();
	
};