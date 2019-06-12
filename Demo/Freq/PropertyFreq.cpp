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

#include "PropertyFreq.h"

void QtnPropertyFreqBase::setUnit(FreqUnit unit)
{
	if (m_unit == unit)
		return;

	Q_EMIT propertyWillChange(QtnPropertyChangeReasonValue, &unit, 0);

	m_unit = unit;

	Q_EMIT propertyDidChange(QtnPropertyChangeReasonValue);
}

bool QtnPropertyFreqBase::fromStrImpl(
	const QString &str, QtnPropertyChangeReason reason)
{
	Q_UNUSED(str);
	Q_UNUSED(reason);
	return false;
}

bool QtnPropertyFreqBase::toStrImpl(QString &str) const
{
	Q_UNUSED(str);
	return false;
}
