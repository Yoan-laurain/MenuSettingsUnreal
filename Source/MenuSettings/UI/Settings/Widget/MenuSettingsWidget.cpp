#include "MenuSettingsWidget.h"
#include "ValidationPopUpWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Navigation/NavigationButtonWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Description/SettingsDescription.h"
#include "Components/ProgressBar/SettingsProgressBarWidget.h"
#include "Components/Title/SettingsCategoryTitleWidget.h"
#include "Components/Basic/SettingsWidget.h"
#include "Components/VerticalBox.h"
#include "MenuSettings/Player//LocalPlayerCustom.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

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
	
	if ( ApplyButton )
	{
		ApplyButton->OnClicked.AddDynamic(this, &UMenuSettingsWidget::CreatePopUpValidation);
	}

	if ( CancelButton )
	{
		CancelButton->OnClicked.AddDynamic(this, &UMenuSettingsWidget::Cancel);
	}
}

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
							UiSettingsParentClass* SettingsWidget = NULL;
							
							if ( Setting->GetType() == ESettingsType::Normal )
							{
								SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsItemWidgetClass);
							}
							else
							{
								SettingsWidget = CreateWidget<USettingsProgressBarWidget>(GetWorld(), SettingsProgressBarWidgetClass);
							}

							if ( SettingsWidget )
							{
								SettingsWidget->InitWidget(Setting);
								SettingsWidget->SetParentWidget(this);
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

void UMenuSettingsWidget::OnNavigationButtonClicked(const FString SettingsName)
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

void UMenuSettingsWidget::ChangeDescription(const FText Description)
{
	SettingsDescriptionWidget->SetDescriptionText(Description);
}

void UMenuSettingsWidget::CreatePopUpValidation()
{
	UValidationPopUpWidget* ValidationPopUpWidget = CreateWidget<UValidationPopUpWidget>(GetWorld(), ValidationPopUpWidgetClass);
	ValidationPopUpWidget->SetMenuSettingsWidget(this);
	ValidationPopUpWidget->AddToViewport();
}

void UMenuSettingsWidget::ApplySettings()
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->SaveChanges();
}

void UMenuSettingsWidget::Cancel()
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->CancelChanges();
}