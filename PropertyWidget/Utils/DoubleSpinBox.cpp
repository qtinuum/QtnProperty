#include "DoubleSpinBox.h"

QtnDoubleSpinBox::QtnDoubleSpinBox(QWidget *parent)
	: QDoubleSpinBox(parent)
	, percent_suffix(false)
{

}

bool QtnDoubleSpinBox::getHavePercentSuffix() const
{
	return percent_suffix;
}

void QtnDoubleSpinBox::setHavePercentSuffix(bool value)
{
	if (percent_suffix != value)
	{
		percent_suffix = value;
		update();
	}
}

QValidator::State QtnDoubleSpinBox::validate(QString &input, int &pos) const
{
	if (percent_suffix)
	{
		auto value = getNumberText(input);

		auto result = QDoubleSpinBox::validate(value, pos);

		if (QValidator::Acceptable == result)
		{
			value.append(QLocale().percent());
			input = value;
		}

		return result;
	}

	return QDoubleSpinBox::validate(input, pos);
}

double QtnDoubleSpinBox::valueFromText(const QString &text) const
{
	return QDoubleSpinBox::valueFromText(getNumberText(text));
}

QString QtnDoubleSpinBox::textFromValue(double val) const
{
	auto result = QDoubleSpinBox::textFromValue(val);

	QLocale locale;
	auto decimal_point = locale.decimalPoint();
	auto group_separator = locale.groupSeparator();
	auto zero_digit = locale.zeroDigit();
	int i = result.indexOf(decimal_point);
	if (i >= 0)
	{
		auto begin = result.constData();
		auto data = &begin[result.length() - 1];
		auto dec_begin = &begin[i];

		while (data >= dec_begin && (	*data == zero_digit
									||	*data == decimal_point
									||	*data == group_separator))
			data--;

		result.resize(data + 1 - begin);
	}

	if (percent_suffix)
		result.append(locale.percent());

	return result;
}

QString QtnDoubleSpinBox::getNumberText(const QString &source) const
{
	auto result = source;
	if (percent_suffix)
	{
		int new_len = result.length() - 1;
		if (new_len > 0)
		{
			auto c = result.at(new_len);
			if (c == '%' || c == QLocale().percent())
				result.resize(new_len);
		}
	}

	return result;
}
