#pragma once

#include "Blueprint/UserWidget.h"
#include "NavigationButtonWidget.generated.h"

UCLASS()
class MENUSETTINGS_API UNavigationButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public :

	/** The button to navigate */
	UPROPERTY( BlueprintReadWrite, meta = (BindWidget) )
	class UButton* NavigationButton;

	/** The text of teh button */
	UPROPERTY( BlueprintReadWrite, meta = (BindWidget) )
	class UTextBlock* NavigationText;

	void InitWidget(FString Text);
	
	DECLARE_DELEGATE(FNavigationButtonClickedDelegate)
	FNavigationButtonClickedDelegate NavigationButtonClickedDelegate;

protected :
	
	UFUNCTION()
	void OnNavigationButtonClicked();
};