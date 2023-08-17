#pragma once

#include "Blueprint/UserWidget.h"
#include "../UiSettingsParentClass.h"
#include "BindingKeyWidget.generated.h"

enum class ECommonInputType : uint8;
class UChooseAKeyWidget;
class UTextBlock;
class UButtonBase;
class UImage;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class MENUSETTINGS_API UBindingKeyWidget : public UiSettingsParentClass
{
	GENERATED_BODY()

public :

	// UiSettingsParentClass implementation Begin
	virtual void UpdateHUD() override;
	// UiSettingsParentClass implementation End

	virtual void SetCurrentValue(FText NewText) override;
	virtual void SetCurrentValue(const FKey& Key);

#pragma region WidgetComponents
	
	UPROPERTY(meta = (BindWidget),EditAnywhere,BlueprintReadWrite)
	UButtonBase* Button;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChooseAKeyWidgetClass;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BindWidget))
	UImage* IconImage;

#pragma endregion WidgetComponents

	virtual void Refresh();
	void SetInternalFocus();
	virtual void InitWidget(UGameSettingsItem* SettingsItem) override;
	
	void SetTypeInputExpected(ECommonInputType InTypeInputExpected);

	virtual FSlateBrush GetIconWithName(const FKey& Key) const;

	virtual UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
	
private :

	UPROPERTY()
	UChooseAKeyWidget* ChooseAKeyWidget;

	ECommonInputType ExpectedInputType;
	
protected :
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnTryKeyChange();
};