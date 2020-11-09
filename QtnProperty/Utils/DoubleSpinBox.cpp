/*******************************************************************************
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

QString QtnDoubleSpinBox::valueToText(
	double value, const QLocale &locale, int decimals, bool groupSeparatorShown)
{
	auto result = locale.toString(value, 'f', decimals);

	auto groupSeparator = locale.groupSeparator();

	if (!groupSeparatorShown)
		result.remove(groupSeparator);

	auto decimalPoint = locale.decimalPoint();
	int i = result.indexOf(decimalPoint);

	if (i >= 0)
	{
		auto digitsTest = QByteArray::number(
			value, 'f', std::numeric_limits<double>::digits10 - 1);
		if (digitsTest.endsWith('9') && result.endsWith(locale.toString(9)))
		{
			auto begin = result.constData();
			auto data = &begin[result.length() - 1];
			auto decBegin = &begin[i];

			while (data >= decBegin &&
				(data->digitValue() == 9 || *data == decimalPoint ||
					*data == groupSeparator))
			{
				data--;
			}

			int newLength = int(data + 1 - begin);
			if (result.length() - newLength > 1)
			{
				result.resize(newLength);
				int last = newLength - 1;
				auto digit = result[last].digitValue();
				if (digit == 9)
				{
					result.replace(last, 1, locale.toString(0));
					int prev = last - 1;
					if (last == 0 || !result[prev].isDigit())
					{
						result.insert(last, locale.toString(1));
					} else
					{
						result.replace(prev, 1,
							locale.toString(result[prev].digitValue() + 1));
					}
				} else if (digit >= 0)
				{
					result.replace(last, 1, locale.toString(digit + 1));
				}
			}
		} else
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
	}

	return result;
}
