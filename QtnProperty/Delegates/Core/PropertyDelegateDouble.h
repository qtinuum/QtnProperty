/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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
#include "QtnProperty/Core/PropertyDouble.h"

class QTN_IMPORT_EXPORT QtnPropertyDelegateDouble
	: public QtnPropertyDelegateTyped<QtnPropertyDoubleBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateDouble)

	QString m_suffix;
	double m_multiplier;
	int m_precision;
	QVariant m_min;
	QVariant m_max;
	QVariant m_step;

public:
	QtnPropertyDelegateDouble(QtnPropertyDoubleBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

	double stepValue() const;
	double minValue() const;
	double maxValue() const;
	double multiplier() const;
	double currentValue() const;

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual bool acceptKeyPressedForInplaceEditImpl(
		QKeyEvent *keyEvent) const override;

	virtual bool propertyValueToStrImpl(QString &strValue) const override;

	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;
};
