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

#ifndef PROPERTY_AB_COLOR_H
#define PROPERTY_AB_COLOR_H

#include "QtnProperty/GUI/PropertyQColor.h"

class QtnPropertyABColor : public QtnPropertyQColor
{
	Q_OBJECT
	QtnPropertyABColor(const QtnPropertyABColor &other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyABColor(QObject *parent)
		: QtnPropertyQColor(parent)
	{
	}

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyABColor, QtnPropertyQColor)

	void invokeClick();
	void setClickHandler(
		const std::function<void(const QtnPropertyABColor *)> &clickHandler);

Q_SIGNALS:
	void click(const QtnPropertyABColor *property);
};

struct MyColor
{
    int red = 0;
    int green = 0;
    int blue = 0;
};

using QtnPropertyMyColorBase = QtnSinglePropertyBaseAs<QtnPropertyQColorBase, MyColor>;

class QtnPropertyMyColor : public QtnSinglePropertyValue<QtnPropertyMyColorBase>
{
    Q_OBJECT

private:
    QtnPropertyMyColor(const QtnPropertyMyColor &other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyMyColor(QObject *parent = nullptr)
        : QtnSinglePropertyValue<QtnPropertyMyColorBase>(parent)
    {
    }

protected:
    bool fromActualValue(ValueType actualValue, BaseValueTypeStore& baseValue) const override;
    bool toActualValue(ValueTypeStore& actualValue, BaseValueType baseValue) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyMyColor, QtnPropertyMyColorBase)
};

#endif // PROPERTY_AB_COLOR_H
