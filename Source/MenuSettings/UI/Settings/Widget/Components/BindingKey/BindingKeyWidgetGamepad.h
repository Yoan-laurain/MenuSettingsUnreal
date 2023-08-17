#pragma once

#include "BindingKeyWidget.h"
#include "BindingKeyWidgetGamepad.generated.h"

class UImage;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class MENUSETTINGS_API UBindingKeyWidgetGamepad : public UBindingKeyWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BindWidget))
	UImage* IconImage;

	virtual void Refresh() override;
	
	virtual void SetCurrentValue(const FKey& Key);
	
	FSlateBrush GetIconWithName(const FKey& Key) const;

	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
	
};
