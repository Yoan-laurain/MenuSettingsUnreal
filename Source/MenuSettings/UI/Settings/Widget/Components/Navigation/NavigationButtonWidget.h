#pragma once

#include "Blueprint/UserWidget.h"
#include "NavigationButtonWidget.generated.h"

class UGameSettingsCollection;
class UButton;
class UTextBlock;

UCLASS()
class MENUSETTINGS_API UNavigationButtonWidget final : public UUserWidget
{
	GENERATED_BODY()

public :

#pragma region WidgetComponents
	
	/** The button to navigate */
	UPROPERTY( BlueprintReadWrite,EditAnywhere, meta = (BindWidget) )
	UButton* NavigationButton;

	/** The text of the button */
	UPROPERTY( BlueprintReadWrite,EditAnywhere, meta = (BindWidget) )
	UTextBlock* NavigationText;

#pragma endregion WidgetComponents

	void InitWidget(const UGameSettingsCollection* Setting);

	UFUNCTION()
	void OnNavigationButtonClicked();

	UFUNCTION(BlueprintImplementableEvent)
	void SetHoverState(const bool bHover);
	
	DECLARE_DELEGATE(FNavigationButtonClickedDelegate)
	FNavigationButtonClickedDelegate NavigationButtonClickedDelegate;
};