#include "GameSettingValueDiscreteDynamic.h"

#include "GameSettingDataSource.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

UGameSettingValueDiscreteDynamic::UGameSettingValueDiscreteDynamic()
{
}

void UGameSettingValueDiscreteDynamic::SetDynamicGetter(const TSharedRef<FGameSettingDataSource>& InGetter)
{
	Getter = InGetter;
}

void UGameSettingValueDiscreteDynamic::SetDynamicSetter(const TSharedRef<FGameSettingDataSource>& InSetter)
{
	Setter = InSetter;
}

void UGameSettingValueDiscreteDynamic::SetDefaultValueFromString(FString InOptionValue)
{
	DefaultValue = InOptionValue;
}

void UGameSettingValueDiscreteDynamic::AddDynamicOption(FString InOptionValue, FText InOptionText)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!OptionValues.Contains(InOptionValue), TEXT("You already added this option InOptionValue: %s InOptionText %s."), *InOptionValue, *InOptionText.ToString());
#endif

	OptionValues.Add(InOptionValue);
	OptionDisplayTexts.Add(InOptionText);
}

void UGameSettingValueDiscreteDynamic::RemoveDynamicOption(FString InOptionValue)
{
	const int32 Index = OptionValues.IndexOfByKey(InOptionValue);
	if (Index != INDEX_NONE)
	{
		OptionValues.RemoveAt(Index);
		OptionDisplayTexts.RemoveAt(Index);
	}
}

const TArray<FString>& UGameSettingValueDiscreteDynamic::GetDynamicOptions()
{
	return OptionValues;
}

bool UGameSettingValueDiscreteDynamic::HasDynamicOption(const FString& InOptionValue)
{
	return OptionValues.Contains(InOptionValue);
}

FString UGameSettingValueDiscreteDynamic::GetValueAsString() const
{
	const USettingsManager * SettingsManager = USettingsManager::Get();
	return Getter->GetValueAsString(SettingsManager->GetLocalPlayer());
}

void UGameSettingValueDiscreteDynamic::SetValueFromString(FString InStringValue)
{
	check(Setter);

	USettingsManager * SettingsManager = USettingsManager::Get();
	
	Setter->SetValue(SettingsManager->GetLocalPlayer(), InStringValue);
}

bool UGameSettingValueDiscreteDynamic::AreOptionsEqual(const FString& InOptionA, const FString& InOptionB) const
{
	return InOptionA == InOptionB;
}

void UGameSettingValueDiscreteDynamic::OnInitialized()
{
	Super::OnInitialized();
}

void UGameSettingValueDiscreteDynamic::StoreInitial()
{
	InitialValue = GetValueAsString();
}

void UGameSettingValueDiscreteDynamic::ResetToDefault()
{
	if (DefaultValue.IsSet())
	{
		SetValueFromString(DefaultValue.GetValue());
	}
}

void UGameSettingValueDiscreteDynamic::RestoreToInitial()
{
	SetValueFromString(InitialValue);
}

void UGameSettingValueDiscreteDynamic::SetDiscreteOptionByIndex(int32 Index)
{
	if (ensure(OptionValues.IsValidIndex(Index)))
	{
		SetValueFromString(OptionValues[Index]);
	}
}

int32 UGameSettingValueDiscreteDynamic::GetDiscreteOptionIndex() const
{
	const FString CurrentValue = GetValueAsString();
	const int32 Index = OptionValues.IndexOfByPredicate([this, CurrentValue](const FString& InOption) {
		return AreOptionsEqual(CurrentValue, InOption);
	});

	// If we can't find the correct index, send the default index.
	if (Index == INDEX_NONE)
	{
		return GetDiscreteOptionDefaultIndex();
	}

	return Index;
}

int32 UGameSettingValueDiscreteDynamic::GetDiscreteOptionDefaultIndex() const
{
	if (DefaultValue.IsSet())
	{
		return OptionValues.IndexOfByPredicate([this](const FString& InOption) {
			return AreOptionsEqual(DefaultValue.GetValue(), InOption);
		});
	}

	return INDEX_NONE;
}

TArray<FText> UGameSettingValueDiscreteDynamic::GetDiscreteOptions() const
{
	return OptionDisplayTexts;
}

//////////////////////////////////////////////////////////////////////////
// UGameSettingValueDiscreteDynamic_Bool
//////////////////////////////////////////////////////////////////////////

UGameSettingValueDiscreteDynamic_Bool::UGameSettingValueDiscreteDynamic_Bool()
{
	AddDynamicOption(TEXT("false"), FText::FromString(TEXT("OFF")));
	AddDynamicOption(TEXT("true"), FText::FromString(TEXT("ON")));
}

void UGameSettingValueDiscreteDynamic_Bool::SetTrueText(const FText& InText)
{
	// We remove and then re-add it, so that by changing the true/false text you can also control the order they appear.
	RemoveDynamicOption(TEXT("true"));
	AddDynamicOption(TEXT("true"), InText);
}

void UGameSettingValueDiscreteDynamic_Bool::SetFalseText(const FText& InText)
{
	// We remove and then re-add it, so that by changing the true/false text you can also control the order they appear.
	RemoveDynamicOption(TEXT("false"));
	AddDynamicOption(TEXT("false"), InText);
}

void UGameSettingValueDiscreteDynamic_Bool::SetDefaultValue(bool Value)
{
	DefaultValue = LexToString(Value);
}

//////////////////////////////////////////////////////////////////////////
// UGameSettingValueDiscreteDynamic_Number
//////////////////////////////////////////////////////////////////////////

UGameSettingValueDiscreteDynamic_Number::UGameSettingValueDiscreteDynamic_Number()
{

}

void UGameSettingValueDiscreteDynamic_Number::OnInitialized()
{
	Super::OnInitialized();

	ensure(OptionValues.Num() > 0);
}

//////////////////////////////////////////////////////////////////////////
// UGameSettingValueDiscreteDynamic_Enum
//////////////////////////////////////////////////////////////////////////

UGameSettingValueDiscreteDynamic_Enum::UGameSettingValueDiscreteDynamic_Enum()
{

}

void UGameSettingValueDiscreteDynamic_Enum::OnInitialized()
{
	Super::OnInitialized();

	ensure(OptionValues.Num() > 0);
}

#undef LOCTEXT_NAMESPACE
