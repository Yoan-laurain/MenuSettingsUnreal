#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuSettingsWidget.generated.h"

class UGameSettingsCollection;

UCLASS()
class MENUSETTINGS_API UMenuSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public :

	/** Where options are displayed */
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SettingsScrollBox;

	/** Where the navigations buttons are put */
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* NavigationButtonsBox;

	/** The widget of a unique settings */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsItemWidgetClass;

	/** The widget of a category title */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsCategoryTitleWidgetClass;

	/** The widget for navigations */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsNavigationWidgetClass;
	
	void OnNavigationButtonClicked(FString SettingsName);
	
private :

	void SetContent(UGameSettingsCollection* SettingsCollection);
	void CreateSubTitle(FText Title);
	void CreateSectionsButtons(TArray<FString>* NavigationButtons);
	
	FString CurrentMenuName;

protected :

	virtual void NativeOnInitialized() override;
	
};