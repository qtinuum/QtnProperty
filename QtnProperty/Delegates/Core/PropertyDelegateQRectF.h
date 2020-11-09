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

#ifndef PROPERTY_DELEGATE_QRECTF_H
#define PROPERTY_DELEGATE_QRECTF_H

#include "QtnProperty/Delegates/Utils/PropertyDelegateMisc.h"
#include "QtnProperty/Core/PropertyQRectF.h"

class QTN_IMPORT_EXPORT QtnPropertyDelegateQRectF
	: public QtnPropertyDelegateTypedEx<QtnPropertyQRectFBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQRectF)

	bool m_coordinates;
	int m_precision;

public:
	QtnPropertyDelegateQRectF(
		QtnPropertyQRectFBase &owner, bool useCoordinates = false);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual bool propertyValueToStrImpl(QString &strValue) const override;

private:
	enum
	{
		LEFT = 0,
		TOP = 1,
		WIDTH = 2,
		HEIGHT = 3,
		RIGHT = 2,
		BOTTOM = 3
	};
};

#endif // PROPERTY_DELEGATE_QRECTF_H
