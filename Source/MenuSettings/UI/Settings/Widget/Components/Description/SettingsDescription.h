#pragma once

#include "Blueprint/UserWidget.h"
#include "SettingsDescription.generated.h"

class UTextBlock;

UCLASS()
class MENUSETTINGS_API USettingsDescription final : public UUserWidget 
{ 
	GENERATED_BODY()

public :

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

	void SetDescriptionText(const FText& NewText);
	void SetTitleText(const FText& NewText);
};