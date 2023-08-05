#pragma once

#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "MenuSettings/UI/Foundation/CustomActivatableWidget.h"
#include "MenuSettingsWidget.generated.h"

class UiSettingsParentClass;
class UButtonBase;
class UVerticalBox;
class UGameSettingsCollection;
class UButton;
class UScrollBox;
class UHorizontalBox;
class USettingsDescription;
class UNavigationButtonsContainer;
class UValidationPopUpWidget;

UCLASS()
class MENUSETTINGS_API UMenuSettingsWidget final : public UCustomActivatableWidget
{
	GENERATED_BODY()

public :

#pragma region WidgetComponents
	
	/** Where options are displayed */
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	UScrollBox* SettingsScrollBox;

	/** The container for description */
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Right;

	/** Where the navigations buttons are put */
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	UNavigationButtonsContainer* NavigationButtonsContainer;

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
	
	void OnNavigationButtonClicked(const FString& SettingsName);
	void ChangeDescription(const FText& Description,const FText& SettingName);
	void SetPendingModificationState(const bool bIsEnabled);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnSettingsDirtyStateChanged(bool bSettingsDirty);
	void OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty);
	
	void SetFocusInternal();

	virtual UWidget* NativeGetDesiredFocusTarget() const override;

#pragma region PopUpMethods

	UFUNCTION(BlueprintCallable)
	void CreatePopUpValidation(const bool bShouldCloseMenuSettings);

	UFUNCTION()
	void ApplySettings();
	
	void Cancel();

	UFUNCTION(BlueprintImplementableEvent)
	void Close();

#pragma endregion 

#pragma region Actions

	void OnCloseClicked();
	void ResetValues();
	void CallPopUpInternal();
	
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ApplyInputActionData;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ResetChangesInputActionData;

	FUIActionBindingHandle BackHandle;
	FUIActionBindingHandle ApplyHandle;
	FUIActionBindingHandle ResetChangesHandle;

#pragma endregion Actions
	
	void SetItemToFocusAtFirst(UWidget* ItemToFocusAtFirst);
	
private :

	void SetContent(UGameSettingsCollection* SettingsCollection);
	void CreateSubTitle(const FText& Title);
	void CreateSectionsButtons(TArray<FString>* NavigationButtons);
	
	FString CurrentMenuName;
	
protected :

	UPROPERTY()
	UWidget* ItemToFocusAtFirst;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};