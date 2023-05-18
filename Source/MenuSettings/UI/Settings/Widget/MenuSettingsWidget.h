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

	/** The widget of a unique settings */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsItemWidgetClass;

	/** The widget of a category title */
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> SettingsCategoryTitleWidgetClass;
	
private :

	void SetContent(UGameSettingsCollection* SettingsCollection);
	void CreateSubTitle(FText Title);

protected :

	virtual void NativeOnInitialized() override;
};