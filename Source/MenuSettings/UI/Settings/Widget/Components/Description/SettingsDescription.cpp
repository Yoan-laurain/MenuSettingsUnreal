#include "MenuSettings/UI/Settings/Widget/Components/Description/SettingsDescription.h"
#include "Components/TextBlock.h"

void USettingsDescription::SetDescriptionText(const FText NewText)
{
	if ( DescriptionText )
	{
		DescriptionText->SetText(NewText);
	}
}