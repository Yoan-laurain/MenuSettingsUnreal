#include "MenuSettingsWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/NavigationButtonWidget.h"
#include "Components/ScrollBox.h"
#include "Components/SettingsCategoryTitleWidget.h"
#include "Components/SettingsWidget.h"
#include "MenuSettings/UI/Settings/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/LocalPlayerCustom.h"
#include "MenuSettings/UI/Settings/SettingsManager.h"

void UMenuSettingsWidget::SetContent(UGameSettingsCollection* SettingsCollection)
{
	if ( SettingsCollection )
	{
		CurrentMenuName = SettingsCollection->GetTitle().ToString();
		
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

void UMenuSettingsWidget::CreateSectionsButtons(TArray<FString>* NavigationButtons)
{
	if ( NavigationButtons )
	{
		for ( const auto& Button : *NavigationButtons )
		{
			UNavigationButtonWidget* SettingsWidget = CreateWidget<UNavigationButtonWidget>(GetWorld(), SettingsNavigationWidgetClass);
			
			 if ( SettingsWidget )
			 {
			 	SettingsWidget->NavigationButtonClickedDelegate.BindLambda( [this, Button] () { OnNavigationButtonClicked(Button); } );
			 	
			 	NavigationButtonsBox->AddChild(SettingsWidget);
			 	
			 	SettingsWidget->InitWidget(Button);
			 }
		}
	}
}

void UMenuSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	USettingsManager* SettingsManager = USettingsManager::Get();
	
	if ( SettingsManager )
	{
		if (ULocalPlayerCustom* InLocalPlayer = Cast<ULocalPlayerCustom>(GetOwningLocalPlayer()) )
		{
			SettingsManager->OnInitialize(InLocalPlayer);
		}
		
		CreateSectionsButtons(SettingsManager->InitializeNavigationsButtons());
		
		SetContent(SettingsManager->GetVideoSettings());
	}
}

void UMenuSettingsWidget::OnNavigationButtonClicked(FString SettingsName)
{
	if ( CurrentMenuName == SettingsName )
	{
		return;
	}
	
	const USettingsManager* SettingsManager = USettingsManager::Get();

	if ( SettingsManager )
	{
		SettingsScrollBox->ClearChildren();
		SetContent(SettingsManager->GetSettings(SettingsName));
	}
}