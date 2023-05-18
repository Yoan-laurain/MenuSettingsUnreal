#include "MenuSettingsWidget.h"
#include "Components/ScrollBox.h"
#include "Components/SettingsCategoryTitleWidget.h"
#include "Components/SettingsWidget.h"
#include "MenuSettings/UI/Settings/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/SettingsManager.h"

void UMenuSettingsWidget::SetContent(UGameSettingsCollection* SettingsCollection)
{
	if ( SettingsCollection )
	{
		CreateSubTitle(SettingsCollection->GetTitle());
		
		for ( const auto& SettingCol : SettingsCollection->GetChildSettingsCollection() )
		{
			if ( SettingCol )
			{
				CreateSubTitle(SettingCol->GetTitle());
				
				if ( SettingCol->GetChildSettingsCollection().IsEmpty() )
				{
					for ( const auto& Setting : SettingCol->GetChildSettings() )
					{
						if ( Setting )
						{
							USettingsWidget* SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsItemWidgetClass);
					
							if ( SettingsWidget )
							{
								SettingsWidget->InitWidget(Setting);
								SettingsScrollBox->AddChild(SettingsWidget);
							}
						}
					}
				}
			}
		}
	}
}

void UMenuSettingsWidget::CreateSubTitle(const FText Title)
{
	USettingsCategoryTitleWidget* SettingsCategoryTitleWidget = CreateWidget<USettingsCategoryTitleWidget>(GetWorld(), SettingsCategoryTitleWidgetClass);

	if ( SettingsCategoryTitleWidget )
	{
		SettingsCategoryTitleWidget->SetTitle(Title);
		SettingsScrollBox->AddChild(SettingsCategoryTitleWidget);
	}
}

void UMenuSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const USettingsManager* SettingsManager = USettingsManager::Get(GetOwningLocalPlayer());

	if ( SettingsManager )
	{
		SetContent(SettingsManager->GetVideoSettings());
	}
}