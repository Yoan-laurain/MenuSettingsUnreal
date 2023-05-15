#include "MenuSettingsWidget.h"
#include "SettingsWidget.h"
#include "Components/VerticalBox.h"
#include "MenuSettings/UI/Settings/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/GeneralSettings.h"

void UMenuSettingsWidget::SetContent(UGameSettingsCollection* SettingsCollection)
{
	if ( SettingsCollection )
	{
		for ( const auto& Setting : SettingsCollection->GetChildSettings() )
		{
			if ( Setting )
			{
				USettingsWidget* SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsWidgetClass);
				SettingsWidget->InitWidget();

				if ( SettingsWidget )
				{
					LeftSideBox->AddChildToVerticalBox(SettingsWidget);
				}
			}
		}
	}
}

void UMenuSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGeneralSettings* GeneralSettings = UGeneralSettings::Get();

	if ( GeneralSettings )
	{
		SetContent(GeneralSettings->InitializeVideoSettings());
	}
}