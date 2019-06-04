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
	auto zeroDigit = locale.zeroDigit();
	int i = result.indexOf(decimalPoint);

	if (i >= 0)
	{
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
