#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuSettingsWidget.generated.h"

class UVerticalBox;
class UGameSettingsCollection;
class UButton;
class UScrollBox;
class UHorizontalBox;
class USettingsDescription;

UCLASS()
class MENUSETTINGS_API UMenuSettingsWidget final : public UUserWidget
{
	GENERATED_BODY()

public :

	/** Where options are displayed */
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SettingsScrollBox;

	/** The container for description */
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Right;

	/** Where the navigations buttons are put */
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* NavigationButtonsBox;
	
	UPROPERTY( meta = (BindWidget) )
	UButton* ApplyButton;

	UPROPERTY( meta = (BindWidget) )
	UButton* CancelButton;

	UPROPERTY(meta=(BindWidget) )
	USettingsDescription* SettingsDescriptionWidget;

	/** The widget of a unique settings */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsItemWidgetClass;

	/** The widget for progress bar settings */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsProgressBarWidgetClass;

	/** The widget for input config */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsInputConfigWidgetClass;

	/** The widget of a category title */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsCategoryTitleWidgetClass;

	/** The widget for navigations */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsNavigationWidgetClass;

	/** The widget for description */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsDescriptionWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ValidationPopUpWidgetClass;
	
	void OnNavigationButtonClicked(FString SettingsName);
	void ChangeDescription(FText Description);

	UFUNCTION()
	void CreatePopUpValidation();

	UFUNCTION()
	void ApplySettings();
	
	UFUNCTION()
	void Cancel();
	
	void CancelConfirm(const bool bWithBinding = true);
private :

	void SetContent(UGameSettingsCollection* SettingsCollection);
	void CreateSubTitle(FText Title);
	void CreateSectionsButtons(TArray<FString>* NavigationButtons);
	
	FString CurrentMenuName;
	
protected :

	virtual void NativeOnInitialized() override;

	// destructor
	virtual void NativeDestruct() override;
	
};