#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SettingsWidget.generated.h"

class UImage;
class FText;

UCLASS()
class MENUSETTINGS_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public :

	/** The name of the option */
	UPROPERTY(meta = (BindWidget))
	FText SettingsText;
	
	UPROPERTY(meta = (BindWidget))
	UButton* DecreaseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* IncreaseButton;

	/** The image of the choices that the user can make */
	UPROPERTY(meta = (BindWidget))
	UImage* Image;

	void SetSettingsText(FText NewText);
	void SetImage(unsigned IndexImage);
	void InitWidget();

private :
	
	TMap<unsigned,UImage*> ImageMap;
	unsigned CurrentImageIndex;

protected :
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnDecreaseButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void OnIncreaseButtonClicked();
	
	virtual void NativeOnInitialized() override;
};