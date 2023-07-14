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

	void SetDescriptionText(FText NewText);
};