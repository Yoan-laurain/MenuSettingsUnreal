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
	class UVerticalBox* LeftSideBox;

	/** The widget class of a single settings */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Menu Settings" )
	TSubclassOf<class UUserWidget> SettingsWidgetClass;

private :

	void SetContent(UGameSettingsCollection* SettingsCollection);

protected :

	virtual void NativeOnInitialized() override;
};