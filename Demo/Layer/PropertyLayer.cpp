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

#include "PropertyLayer.h"

QDataStream& operator<< (QDataStream& stream, const LayerInfo& layer)
{
    // version
    stream << (quint8)1;
    stream << layer.name;
    stream << layer.color;
    stream << (quint64)layer.data;
    return stream;
}

QDataStream& operator>> (QDataStream& stream, LayerInfo& layer)
{
    quint8 version = 0;
    stream >> version;
    if (version == 1)
    {
        stream >> layer.name;
        stream >> layer.color;
        quint64 data = 0;
        stream >> data;
        layer.data = (quintptr)data;
    }

    return stream;
}

QList<LayerInfo> QtnPropertyLayerBase::layers() const
{
    if (!m_layersCallback)
        return QList<LayerInfo>();

    return m_layersCallback();
}

void QtnPropertyLayerBase::setLayers(QList<LayerInfo> layers)
{
    m_layersCallback = [layers]() {
        return layers;
    };
}

void QtnPropertyLayerBase::setLayersCallback(std::function<QList<LayerInfo>()> layersCallback)
{
    m_layersCallback = layersCallback;
}

bool QtnPropertyLayerBase::fromStrImpl(const QString& str)
{
    for (const auto& layer : layers())
    {
        if (layer.name == str)
        {
            return setValue(layer);
        }
    }

    return false;
}

bool QtnPropertyLayerBase::toStrImpl(QString& str) const
{
    str = value().name;
    return true;
}
