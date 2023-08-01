#include "MenuSettingsWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Description/SettingsDescription.h"
#include "Components/ProgressBar/SettingsProgressBarWidget.h"
#include "Components/Title/SettingsCategoryTitleWidget.h"
#include "Components/Basic/SettingsWidget.h"
#include "Components/BindingKey/BindingKeyWidget.h"
#include "../../../Player/LocalPlayerCustom.h"
#include "../Category/GameSettingsCollection.h"
#include "Components/Navigation/NavigationButtonWidget.h"
#include "Input/CommonUIInputTypes.h"
#include "../Category/SettingsManager.h"
#include "Components/ValidationPopUp/ValidationPopUpWidget.h"
#include "MenuSettings/UI/Components/ButtonBase.h"

#define LOCTEXT_NAMESPACE "MySettings"

void UMenuSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::CreatePopUpValidation)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::ResetValues)));

	USettingsManager* SettingsManager = USettingsManager::Get();

	if ( ULocalSettings* LocalSettings = ULocalSettings::Get() )
	{
		if ( LocalSettings->ShouldRunAutoBenchmarkAtStartup() )
		{
			LocalSettings->RunAutoBenchmark(true);
		}
	}
	
	if ( SettingsManager )
	{
		if (ULocalPlayerCustom* InLocalPlayer = Cast<ULocalPlayerCustom>(GetOwningLocalPlayer()) )
		{
			SettingsManager->OnInitialize(InLocalPlayer);
		}
		
		CreateSectionsButtons(SettingsManager->InitializeNavigationsButtons());
		
		SetContent(SettingsManager->GetGameplaySettings());
	}

	SetEnabledStateSaveButton(false);
}

void UMenuSettingsWidget::NativeDestruct()
{
	const USettingsManager* SettingsManager = USettingsManager::Get();
	
	if ( SettingsManager->GetHasPendingModifications() )
	{
		CreatePopUpValidation();
	}
	
	Super::NativeDestruct();
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

void UMenuSettingsWidget::CreateSubTitle(const FText& Title)
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

	if ( const USettingsManager* SettingsManager = USettingsManager::Get() )
	{
		SettingsScrollBox->ClearChildren();
		SetContent(SettingsManager->GetSettings(SettingsName));
	}

	if (SettingsDescriptionWidget)
	{
		SettingsDescriptionWidget->SetDescriptionText(FText::FromString(""));
		SettingsDescriptionWidget->SetTitleText(FText::FromString(""));
	}

}

void UMenuSettingsWidget::ChangeDescription(const FText& Description, const FText& SettingName)
{
	SettingsDescriptionWidget->SetDescriptionText(Description);
	SettingsDescriptionWidget->SetTitleText(SettingName);
}

void UMenuSettingsWidget::CreatePopUpValidation()
{
	UValidationPopUpWidget* ValidationPopUpWidget = CreateWidget<UValidationPopUpWidget>(GetWorld(), ValidationPopUpWidgetClass);
	ValidationPopUpWidget->SetMenuSettingsWidget(this);
	ValidationPopUpWidget->SetTitleText(LOCTEXT("UnsavedChanges", "You have unsaved changes. Do you want to save them ?"));
	ValidationPopUpWidget->AddToViewport();
}

void UMenuSettingsWidget::ApplySettings()
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->SaveChanges();
	SetEnabledStateSaveButton(false);
}

void UMenuSettingsWidget::SetEnabledStateSaveButton(const bool bIsEnabledApply)
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->SetHasPendingModifications(bIsEnabledApply);
}

UWidget* UMenuSettingsWidget::NativeGetDesiredFocusTarget() const
{
	if (UWidget* Target = BP_GetDesiredFocusTarget())
	{
		return Target;
	}

	return Super::NativeGetDesiredFocusTarget();
}

void UMenuSettingsWidget::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}

void UMenuSettingsWidget::Cancel()
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->CancelChanges();
	SetEnabledStateSaveButton(false);
}

void UMenuSettingsWidget::ResetValues()
{
	USettingsManager* SettingsManager = USettingsManager::Get();
	SettingsManager->ResetToDefault();
}