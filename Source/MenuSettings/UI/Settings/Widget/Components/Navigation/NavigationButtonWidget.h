#pragma once

#include "Blueprint/UserWidget.h"
#include "NavigationButtonWidget.generated.h"

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

	void InitWidget(FString Text);
	
	DECLARE_DELEGATE(FNavigationButtonClickedDelegate)
	FNavigationButtonClickedDelegate NavigationButtonClickedDelegate;

protected :
	
	UFUNCTION()
	void OnNavigationButtonClicked();
};