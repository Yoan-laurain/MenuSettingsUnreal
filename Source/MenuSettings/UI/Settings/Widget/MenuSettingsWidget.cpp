#include "MenuSettingsWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Navigation/NavigationButtonWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Description/SettingsDescription.h"
#include "Components/ProgressBar/SettingsProgressBarWidget.h"
#include "Components/Title/SettingsCategoryTitleWidget.h"
#include "Components/Basic/SettingsWidget.h"
#include "Components/BindingKey/BindingKeyWidget.h"
#include "../../../Player/LocalPlayerCustom.h"
#include "../Category/GameSettingsCollection.h"
#include "../Category/SettingsManager.h"
#include "Components/Button.h"
#include "Components/ValidationPopUp/ValidationPopUpWidget.h"

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
		
		SetContent(SettingsManager->GetGameplaySettings());
	}
	
	if ( ApplyButton )
	{
		ApplyButton->OnClicked.AddDynamic(this, &UMenuSettingsWidget::CreatePopUpValidation);
	}

	if ( CancelButton )
	{
		CancelButton->OnClicked.AddDynamic(this, &UMenuSettingsWidget::Cancel);
	}

	if ( ResetButton )
	{
		ResetButton->OnClicked.AddDynamic(this, &UMenuSettingsWidget::Reset);
	}
	
}

void UMenuSettingsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	CancelConfirm(false);
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
							
							if ( Setting->GetWidgetType() == ESettingsType::Normal )
							{
								SettingsWidget = CreateWidget<USettingsWidget>(GetWorld(), SettingsItemWidgetClass);
							}
							else if ( Setting->GetWidgetType() == ESettingsType::Slider)
							{
								SettingsWidget = CreateWidget<USettingsProgressBarWidget>(GetWorld(), SettingsProgressBarWidgetClass);
							}
							else if ( Setting->GetWidgetType() == ESettingsType::InputConfig )
							{
								SettingsWidget = CreateWidget<UBindingKeyWidget>(GetWorld(), SettingsInputConfigWidgetClass);
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

void UMenuSettingsWidget::CancelConfirm(const bool bWithBinding)
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->CancelChanges(bWithBinding);
}

void UMenuSettingsWidget::Cancel()
{
	CancelConfirm(true);
}

void UMenuSettingsWidget::Reset()
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->ResetToDefault();
}