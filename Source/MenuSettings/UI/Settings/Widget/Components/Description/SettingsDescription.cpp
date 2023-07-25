#include "SettingsDescription.h"
#include "Components/TextBlock.h"

void USettingsDescription::SetDescriptionText(const FText& NewText)
{
	if ( DescriptionText )
	{
		DescriptionText->SetText(NewText);
	}
}

void USettingsDescription::SetTitleText(const FText& NewText)
{
	if ( TitleText )
	{
		TitleText->SetText(NewText);
	}
}