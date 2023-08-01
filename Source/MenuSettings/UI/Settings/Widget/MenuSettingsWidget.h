#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuSettings/UI/Foundation/LyraActivatableWidget.h"
#include "MenuSettingsWidget.generated.h"

class UButtonBase;
class UVerticalBox;
class UGameSettingsCollection;
class UButton;
class UScrollBox;
class UHorizontalBox;
class USettingsDescription;

UCLASS()
class MENUSETTINGS_API UMenuSettingsWidget final : public ULyraActivatableWidget
{
	GENERATED_BODY()

public :

#pragma region WidgetComponents

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	UButtonBase* ApplyButton;
	
	/** Where options are displayed */
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SettingsScrollBox;

	/** The container for description */
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Right;

	/** Where the navigations buttons are put */
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	UHorizontalBox* NavigationButtonsBox;

	UPROPERTY(meta=(BindWidget) )
	USettingsDescription* SettingsDescriptionWidget;

#pragma endregion WidgetComponents

#pragma region WidgetClasses
	
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

#pragma endregion WidgetClasses
	
	void OnNavigationButtonClicked(FString SettingsName);
	void ChangeDescription(const FText& Description,const FText& SettingName);

	UFUNCTION(BlueprintCallable)
	void CreatePopUpValidation(FText Text);

	UFUNCTION()
	void ApplySettings();
	
	UFUNCTION(BlueprintCallable)
	void Cancel();

	UFUNCTION(BlueprintCallable)
	void Reset();

	void SetEnabledStateSaveButton(const bool bIsEnabled);

	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
private :

	void SetContent(UGameSettingsCollection* SettingsCollection);
	void CreateSubTitle(const FText& Title);
	void CreateSectionsButtons(TArray<FString>* NavigationButtons);
	
	FString CurrentMenuName;
	
protected :

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
};