/*******************************************************************************
Copyright (c) 2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
#pragma once

#include "QtnProperty/Delegates/Utils/PropertyDelegateMisc.h"
#include "QtnProperty/GUI/PropertyQVector3D.h"

class QtnPropertyQVector3DBase;

class QTN_IMPORT_EXPORT QtnPropertyDelegateQVector3D
	: public QtnPropertyDelegateTypedEx<QtnPropertyQVector3DBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQVector3D)

	QString m_suffix;
	double m_multiplier;
	int m_precision;

public:
	QtnPropertyDelegateQVector3D(QtnPropertyQVector3DBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual bool propertyValueToStrImpl(QString &strValue) const override;
};
