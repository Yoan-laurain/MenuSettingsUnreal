#include "LocalSettings.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Category/Bindings/AssetManager/AssetManagerCustom.h"
#include "Internationalization/Culture.h"
#include "UserSettings/EnhancedInputUserSettings.h"

ULocalSettings* ULocalSettings::Get()
{
	return GEngine ? CastChecked<ULocalSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

//////////////////////////////////////////////////////////////////////

void ULocalSettings::ApplyCultureSettings()
{
	if (bResetToDefaultCulture)
	{
		const FCulturePtr SystemDefaultCulture = FInternationalization::Get().GetDefaultCulture();
		check(SystemDefaultCulture.IsValid());

		const FString CultureToApply = SystemDefaultCulture->GetName();
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			// Clear this string
			GConfig->RemoveKey(TEXT("Internationalization"), TEXT("Culture"), GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		bResetToDefaultCulture = false;
	}
	else if (!PendingCulture.IsEmpty())
	{
		// SetCurrentCulture may trigger PendingCulture to be cleared (if a culture change is broadcast) so we take a copy of it to work with
		const FString CultureToApply = PendingCulture;
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			// Note: This is intentionally saved to the users config
			// We need to localize text before the player logs
			GConfig->SetString(TEXT("Internationalization"), TEXT("Culture"), *CultureToApply, GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		ClearPendingCulture();
	}
}

void ULocalSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();
}

void ULocalSettings::ResetCultureToCurrentSettings()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

const FString& ULocalSettings::GetPendingCulture() const
{
	return PendingCulture;
}

void ULocalSettings::SetPendingCulture(const FString& NewCulture)
{
	PendingCulture = NewCulture;
	bResetToDefaultCulture = false;
	bIsDirty = true;
}

void ULocalSettings::OnCultureChanged()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

void ULocalSettings::ClearPendingCulture()
{
	PendingCulture.Reset();
}

bool ULocalSettings::IsUsingDefaultCulture() const
{
	FString Culture;
	GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni);

	return Culture.IsEmpty();
}

void ULocalSettings::ResetToDefaultCulture()
{
	ClearPendingCulture();
	bResetToDefaultCulture = true;
	bIsDirty = true;
}

//////////////////////////////////////////////////////////////////////

void ULocalSettings::RegisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer))
	{
		UAssetManagerCustom& AssetManager = UAssetManagerCustom::Get();
		
		if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			if (UEnhancedInputUserSettings* Settings = EISubsystem->GetUserSettings())
			{
				if ( const UInputBindings* InputBinding = GetDefault<UInputBindings>() )
				{
					for (const FInputMappingContextAndPriority& Entry : InputBinding->InputMappingContexts)
					{
						// Skip entries that don't want to be registered
						if (!Entry.bRegisterWithSettings)
						{
							continue;
						}

						// Register this IMC with the settings!
						if (UInputMappingContext* IMC = AssetManager.GetAsset(Entry.InputMapping))
						{
							Settings->RegisterInputMappingContext(IMC);
						}
					}
				}
			}
		}
	}
}