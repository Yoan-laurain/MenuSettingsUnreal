#pragma once

#include "Internationalization/Text.h"
#include "Math/Range.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "Misc/Optional.h"
#include "Templates/Function.h"
#include "Templates/SharedPointer.h"

#include "GameSettingValueScalarDynamic.generated.h"

class FGameSettingDataSource;
class UObject;

typedef TFunction<FText(double SourceValue, double NormalizedValue)> FSettingScalarFormatFunction;

UCLASS()
class MENUSETTINGS_API UGameSettingValueScalarDynamic : public UGameSettingsItem
{
	GENERATED_BODY()

public:
	UGameSettingValueScalarDynamic();
	
	static FSettingScalarFormatFunction Raw;
	static FSettingScalarFormatFunction RawOneDecimal;
	static FSettingScalarFormatFunction RawTwoDecimals;
	static FSettingScalarFormatFunction ZeroToOnePercent;
	static FSettingScalarFormatFunction ZeroToOnePercent_OneDecimal;
	static FSettingScalarFormatFunction SourceAsPercent1;
	static FSettingScalarFormatFunction SourceAsPercent100;
	static FSettingScalarFormatFunction SourceAsInteger;
	
	void SetDynamicGetter(const TSharedRef<FGameSettingDataSource>& InGetter);
	void SetDynamicSetter(const TSharedRef<FGameSettingDataSource>& InSetter);
	void SetDefaultValue(double InValue);
	void SetDisplayFormat(FSettingScalarFormatFunction InDisplayFormat);

	virtual void SetOptionValue(double Value) override;
	virtual double GetOptionValue() const override;
	virtual double GetOptionSourceStep() const override;

private:
	static const FNumberFormattingOptions& GetOneDecimalFormattingOptions();

protected:

	TSharedPtr<FGameSettingDataSource> Getter;
	TSharedPtr<FGameSettingDataSource> Setter;

	TOptional<double> DefaultValue;
	double InitialValue = 0;

	TRange<double> SourceRange = TRange<double>(0, 1);
	double SourceStep = 0.01;
	TOptional<double> Minimum;
	TOptional<double> Maximum;

	FSettingScalarFormatFunction DisplayFormat;
};