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

const LayerInfo *QtnPropertyLayerBase::valueLayer() const
{
	const auto &layerValues = layers();
	auto layerIndex = value();
	if (layerIndex < 0 || layerIndex >= layerValues.size())
		return nullptr;

	return &layerValues[layerIndex];
}

LayerInfo *QtnPropertyLayerBase::valueLayer()
{
	return const_cast<LayerInfo *>(
		((const QtnPropertyLayerBase *) this)->valueLayer());
}

const QList<LayerInfo> &QtnPropertyLayerBase::layers() const
{
	return m_layers;
}

void QtnPropertyLayerBase::setLayers(QList<LayerInfo> layers)
{
	Q_EMIT propertyWillChange(QtnPropertyChangeReasonValue, nullptr, 0);

	m_layers = layers;

	Q_EMIT propertyDidChange(QtnPropertyChangeReasonValue);
}

bool QtnPropertyLayerBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	int index = 0;
	for (const auto &layer : layers())
	{
		if (layer.name == str)
		{
			return setValue(index, reason);
		}
		++index;
	}

	return false;
}

bool QtnPropertyLayerBase::toStrImpl(QString &str) const
{
	auto layer = valueLayer();
	if (layer)
		str = layer->name;
	return true;
}
