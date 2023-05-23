#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SettingsCategoryTitleWidget.generated.h"

UCLASS()
class MENUSETTINGS_API USettingsCategoryTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public :

	UPROPERTY( meta = (BindWidget) )
	class UTextBlock* TitleTextBlock;
	
	void SetTitle(FText NewTitle) { TitleTextBlock->SetText(NewTitle); }

protected :
	FText Title;
	
};