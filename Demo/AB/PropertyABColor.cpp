/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "PropertyABColor.h"

void QtnPropertyABColor::invokeClick()
{
	emit click(this);
}

void QtnPropertyABColor::setClickHandler(
	const std::function<void(const QtnPropertyABColor *)> &clickHandler)
{
	QObject::connect(this, &QtnPropertyABColor::click, clickHandler);
}

bool QtnPropertyMyColor::fromActualValue(ValueType actualValue, BaseValueTypeStore& baseValue) const
{
    baseValue = QColor::fromRgb(actualValue.red, actualValue.green, actualValue.blue);
    return true;
}

bool QtnPropertyMyColor::toActualValue(ValueTypeStore& actualValue, BaseValueType baseValue) const
{
    actualValue.red = baseValue.red();
    actualValue.green = baseValue.green();
    actualValue.blue = baseValue.blue();
    return true;
}
