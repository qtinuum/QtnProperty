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

#ifndef PROPERTY_PEN_WIDTH_H
#define PROPERTY_PEN_WIDTH_H

#include "Auxiliary/PropertyTemplates.h"
#include <QColor>

enum class PenWidth
{
    Default = 0,
    Thin = 1,
    Middle = 2,
    Thick = 3
};

QDataStream& operator<< (QDataStream& stream, const PenWidth penWidth);
QDataStream& operator>> (QDataStream& stream, PenWidth& penWidth);

Q_DECLARE_METATYPE(PenWidth)

class QtnPropertyPenWidthBase: public QtnSinglePropertyBase<PenWidth>
{
    Q_OBJECT
    QtnPropertyPenWidthBase(const QtnPropertyPenWidthBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyPenWidthBase(QObject *parent)
        : QtnSinglePropertyBase<PenWidth>(parent)
    {
    }

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

private:
    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyPenWidthBase)
};

class QtnPropertyPenWidthCallback: public QtnSinglePropertyCallback<QtnPropertyPenWidthBase>
{
    Q_OBJECT
    QtnPropertyPenWidthCallback(const QtnPropertyPenWidthCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyPenWidthCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyPenWidthBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyPenWidthCallback, QtnPropertyPenWidthBase)
};

class QtnPropertyPenWidth: public QtnSinglePropertyValue<QtnPropertyPenWidthBase>
{
    Q_OBJECT
    QtnPropertyPenWidth(const QtnPropertyPenWidth& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyPenWidth(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyPenWidthBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyPenWidth, QtnPropertyPenWidthBase)
};

#endif // PROPERTY_PEN_WIDTH_H
