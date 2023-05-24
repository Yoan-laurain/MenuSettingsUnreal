#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuSettingsWidget.generated.h"

class UGameSettingsCollection;
class UButton;
class UScrollBox;
class UHorizontalBox;

UCLASS()
class MENUSETTINGS_API UMenuSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public :

	/** Where options are displayed */
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SettingsScrollBox;

	/** Where the navigations buttons are put */
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* NavigationButtonsBox;
	
	UPROPERTY( meta = (BindWidget) )
	UButton* ApplyButton;

	UPROPERTY( meta = (BindWidget) )
	UButton* CancelButton;

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

	void ApplySettings();
	void Cancel();
	
private :

	void SetContent(UGameSettingsCollection* SettingsCollection);
	void CreateSubTitle(FText Title);
	void CreateSectionsButtons(TArray<FString>* NavigationButtons);
	
	FString CurrentMenuName;

protected :

	virtual void NativeOnInitialized() override;
	
};