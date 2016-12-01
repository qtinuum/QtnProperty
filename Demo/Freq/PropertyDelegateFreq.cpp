/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "PropertyDelegateFreq.h"
#include "Delegates/PropertyDelegateFactory.h"
#include "PropertyFreq.h"
#include "Delegates/Utils/PropertyEditorHandler.h"

#include <QComboBox>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QPaintEvent>

void regFreqDelegates()
{
  QtnPropertyDelegateFactory::staticInstance()
    .registerDelegateDefault(&QtnPropertyFreqBase::staticMetaObject
                            , &qtnCreateDelegate<QtnPropertyDelegateFreq, QtnPropertyFreqBase>);
}

static QString freqUnit2Str(FreqUnit unit)
{
    switch (unit) {
    case FreqUnit::Hz:
        return "Hz";
    case FreqUnit::KHz:
        return "KHz";
    case FreqUnit::MHz:
        return "MHz";
    default:
        return "NA";
    }
}

void QtnPropertyDelegateFreq::createSubItemValuesImpl(QtnDrawContext& context, const QRect& rect, QList<QtnSubItem>& subItems)
{
    auto unitWidth = context.painter->fontMetrics().width("MHz");
    auto spaceWidth = context.painter->fontMetrics().width(" ");

    auto valueRect = rect;
    valueRect.setRight(valueRect.right() - unitWidth - spaceWidth);

    auto unitRect = rect;
    unitRect.setLeft(valueRect.right() + spaceWidth);

    if (valueRect.isValid())
        addValueSubItem(context, valueRect, subItems);

    if (unitRect.isValid())
        addUnitSubItem(context, unitRect, subItems);
}

void QtnPropertyDelegateFreq::addValueSubItem(QtnDrawContext& /*context*/, const QRect& rect, QList<QtnSubItem>& subItems)
{
    QtnSubItem subItem(rect);
    subItem.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        auto text = qtnElidedText(*context.painter, QString::number(owner().value()), item.rect, nullptr);
        context.painter->drawText(item.rect, Qt::AlignRight | Qt::AlignVCenter, text);
    };

    subItems.append(subItem);
}

void QtnPropertyDelegateFreq::addUnitSubItem(QtnDrawContext& /*context*/, const QRect& rect, QList<QtnSubItem>& subItems)
{
    QtnSubItem subItem(rect);
    subItem.drawHandler = [this](QtnDrawContext& context, const QtnSubItem& item) {
        qtnDrawValueText(freqUnit2Str(owner().unit()), *context.painter, item.rect, state(context.isActive, item.state()), nullptr);
    };

    subItems.append(subItem);

}
