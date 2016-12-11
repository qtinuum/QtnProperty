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

#ifndef PROPERTY_LAYER_H
#define PROPERTY_LAYER_H

#include "Auxiliary/PropertyTemplates.h"
#include <QColor>

struct LayerInfo
{
    QString name;
    QColor color;
    qintptr data = 0;

    LayerInfo() {}
    LayerInfo(QString name, QColor color, qintptr data = 0)
        : name(name), color(color), data(data)
    {}
};

/*
inline bool operator== (const LayerInfo& left, const LayerInfo& right) { return left.name == right.name; }
inline bool operator != (const LayerInfo& left, const LayerInfo& right) { return !(left == right); }

QDataStream& operator<< (QDataStream& stream, const LayerInfo& layer);
QDataStream& operator>> (QDataStream& stream, LayerInfo& layer);

Q_DECLARE_METATYPE(LayerInfo)
*/

class QtnPropertyLayerBase: public QtnSinglePropertyBase<int>
{
    Q_OBJECT
    QtnPropertyLayerBase(const QtnPropertyLayerBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyLayerBase(QObject *parent)
        : QtnSinglePropertyBase<int>(parent)
    {
    }

    const LayerInfo* valueLayer() const;
    LayerInfo* valueLayer();

    const QList<LayerInfo>& layers() const;
    void setLayers(QList<LayerInfo> layers);

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

private:
    QList<LayerInfo> m_layers;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyLayerBase)
};

class QtnPropertyLayerCallback: public QtnSinglePropertyCallback<QtnPropertyLayerBase>
{
    Q_OBJECT
    QtnPropertyLayerCallback(const QtnPropertyLayerCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyLayerCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyLayerBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyLayerCallback, QtnPropertyLayerBase)
};

class QtnPropertyLayer: public QtnSinglePropertyValue<QtnPropertyLayerBase>
{
    Q_OBJECT
    QtnPropertyLayer(const QtnPropertyLayer& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyLayer(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyLayerBase>(parent)
    {
    }

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyLayer, QtnPropertyLayerBase)
};

#endif // PROPERTY_LAYER_H
