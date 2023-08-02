#pragma once

#include "CommonButtonBase.h"
#include "NavigationButtonsContainer.generated.h"

class UHorizontalBox;
class UNavigationButtonWidget;
class UImage;

UCLASS()
class MENUSETTINGS_API UNavigationButtonsContainer final : public UCommonButtonBase
{

	GENERATED_BODY()
	
public:

#pragma region WidgetComponents
	
	UPROPERTY( BlueprintReadWrite,EditAnywhere, meta = (BindWidget) )
	UHorizontalBox* NavigationButtonsBox;
	
	UPROPERTY( BlueprintReadWrite,EditAnywhere, meta = (BindWidget) )
	UImage* LeftButtonImage;
	
	UPROPERTY( BlueprintReadWrite,EditAnywhere, meta = (BindWidget) )
	UImage* RightButtonImage;

#pragma endregion WidgetComponents

	void AddNavigationButton(UNavigationButtonWidget* Button);

	void OnLeftButtonClicked();
	void OnRightButtonClicked();
	
	void SetNormalStateForOtherTabs();
	void HandleImageVisibility(bool bVisible);

	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
	
private :
	
	UPROPERTY()
	UNavigationButtonWidget* ActiveTab;
	
protected :

	virtual void NativeConstruct() override;
};