#include "GameSettingValueScalarDynamic.h"
#include "GameSettingDataSource.h"
#include "MenuSettings/UI/Settings/SettingsManager.h"

#define LOCTEXT_NAMESPACE "GameSetting"

UGameSettingValueScalarDynamic::UGameSettingValueScalarDynamic()
{
}

//////////////////////////////////////////////////////////////////////////
// SettingScalarFormats
//////////////////////////////////////////////////////////////////////////

static FText PercentFormat = LOCTEXT("PercentFormat", "{0}%");

FSettingScalarFormatFunction UGameSettingValueScalarDynamic::Raw([](double SourceValue, double NormalizedValue) {
	return FText::AsNumber(SourceValue);
});

FSettingScalarFormatFunction UGameSettingValueScalarDynamic::RawOneDecimal([](double SourceValue, double NormalizedValue) {
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumIntegralDigits = 1;
	FormattingOptions.MinimumFractionalDigits = 1;
	FormattingOptions.MaximumFractionalDigits = 1;
	return FText::AsNumber(SourceValue, &FormattingOptions);
});

FSettingScalarFormatFunction UGameSettingValueScalarDynamic::RawTwoDecimals([](double SourceValue, double NormalizedValue) {
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumIntegralDigits = 1;
	FormattingOptions.MinimumFractionalDigits = 2;
	FormattingOptions.MaximumFractionalDigits = 2;
	return FText::AsNumber(SourceValue, &FormattingOptions);
});

FSettingScalarFormatFunction UGameSettingValueScalarDynamic::SourceAsInteger([](double SourceValue, double NormalizedValue) {
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumIntegralDigits = 1;
	FormattingOptions.MinimumFractionalDigits = 0;
	FormattingOptions.MaximumFractionalDigits = 0;
	return FText::AsNumber(SourceValue, &FormattingOptions);
});

FSettingScalarFormatFunction UGameSettingValueScalarDynamic::ZeroToOnePercent([](double SourceValue, double NormalizedValue) {
	return FText::Format(PercentFormat, (int32)FMath::RoundHalfFromZero(100.0 * NormalizedValue));
});

FSettingScalarFormatFunction UGameSettingValueScalarDynamic::ZeroToOnePercent_OneDecimal([](double SourceValue, double NormalizedValue) {
	const FNumberFormattingOptions& FormattingOptions = GetOneDecimalFormattingOptions();
	const double NormalizedValueTo100_0 = FMath::RoundHalfFromZero(1000.0 * NormalizedValue);
	return FText::Format(PercentFormat, FText::AsNumber(NormalizedValueTo100_0 / 10.0, &FormattingOptions));
});

const FNumberFormattingOptions& UGameSettingValueScalarDynamic::GetOneDecimalFormattingOptions()
{
	static FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumFractionalDigits = 1;
	FormattingOptions.MaximumFractionalDigits = 1;

	return FormattingOptions;
}

FSettingScalarFormatFunction UGameSettingValueScalarDynamic::SourceAsPercent1([](double SourceValue, double NormalizedValue) {
	return FText::Format(PercentFormat, (int32)FMath::RoundHalfFromZero(100.0 * SourceValue));
});

FSettingScalarFormatFunction UGameSettingValueScalarDynamic::SourceAsPercent100([](double SourceValue, double NormalizedValue) {
	return FText::Format(PercentFormat, (int32)FMath::RoundHalfFromZero(SourceValue));
});


void UGameSettingValueScalarDynamic::SetDynamicGetter(const TSharedRef<FGameSettingDataSource>& InGetter)
{
	Getter = InGetter;
}

void UGameSettingValueScalarDynamic::SetDynamicSetter(const TSharedRef<FGameSettingDataSource>& InSetter)
{
	Setter = InSetter;
}

void UGameSettingValueScalarDynamic::SetDefaultValue(double InValue)
{
	DefaultValue = InValue;
}

void UGameSettingValueScalarDynamic::SetDisplayFormat(FSettingScalarFormatFunction InDisplayFormat)
{
	DisplayFormat = InDisplayFormat;
}

void UGameSettingValueScalarDynamic::SetOptionValue(double InValue)
{
	InValue = FMath::RoundHalfFromZero(InValue / SourceStep);
	InValue = InValue * SourceStep;

	if (Minimum.IsSet())
	{
		InValue = FMath::Max(Minimum.GetValue(), InValue);
	}

	if (Maximum.IsSet())
	{
		InValue = FMath::Min(Maximum.GetValue(), InValue);
	}

	const FString StringValue = LexToString(InValue);

	USettingsManager * SettingsManager = USettingsManager::Get();
	
	Setter->SetValue(SettingsManager->GetLocalPlayer(), StringValue);
}

double UGameSettingValueScalarDynamic::GetOptionValue() const
{
	USettingsManager * SettingsManager = USettingsManager::Get();
	
	const FString OutValue = Getter->GetValueAsString(SettingsManager->GetLocalPlayer());

	double Value;
	LexFromString(Value, *OutValue);

	return Value;
}

double UGameSettingValueScalarDynamic::GetOptionSourceStep() const
{
	return SourceStep;
}