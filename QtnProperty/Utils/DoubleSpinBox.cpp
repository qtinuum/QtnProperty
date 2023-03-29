/*******************************************************************************
Copyright (c) 2015-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include "DoubleSpinBox.h"

QtnDoubleSpinBox::QtnDoubleSpinBox(QWidget *parent)
	: QDoubleSpinBox(parent)
{
}

QString QtnDoubleSpinBox::textFromValue(double val) const
{
	return valueToText(val, locale(), decimals(), isGroupSeparatorShown());
}

QValidator::State QtnDoubleSpinBox::validate(QString& text, int& pos) const
{
	for (auto& chr : text)
		if (chr == QLatin1Char('.') || chr == QLatin1Char(','))
            chr = locale().decimalPoint().front();
	return QDoubleSpinBox::validate(text, pos);
}

QString QtnDoubleSpinBox::valueToText(
	double value, const QLocale &locale, int decimals, bool groupSeparatorShown)
{
	if (!qIsFinite(value))
	{
		return locale.toString(value);
	}

	auto str = QByteArray::number(quint64(qAbs(value)));

	int i = str.length();
	if (!str.startsWith('0'))
	{
		i++;
	}

	int maxDecimals = std::numeric_limits<double>::digits10 - i;
	decimals = std::max(0, std::min(maxDecimals, decimals));

	str = QByteArray::number(value, 'f', decimals);
	if (decimals >= 2 && decimals == maxDecimals &&
		(str.endsWith("99") || str.endsWith("01")))
	{
		decimals--;
	}

	auto result = locale.toString(value, 'f', decimals);
	auto decimalPoint = locale.decimalPoint();
	auto groupSeparator = locale.groupSeparator();
	if (!groupSeparatorShown)
		result.remove(groupSeparator);

	i = result.indexOf(decimalPoint);
	if (i >= 0)
	{
		auto zeroDigit = locale.zeroDigit();
		auto begin = result.constData();
		auto data = &begin[result.length() - 1];
		auto decBegin = &begin[i];

		while (data >= decBegin &&
			(*data == zeroDigit || *data == decimalPoint ||
				*data == groupSeparator))
		{
			data--;
		}

		result.resize(int(data + 1 - begin));
	}

	return result;
}
