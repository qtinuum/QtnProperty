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

#ifndef PROPERTY_DELEGATE_QPEN_H
#define PROPERTY_DELEGATE_QPEN_H

#include "QtnProperty/Delegates/Utils/PropertyDelegateMisc.h"
#include "QtnProperty/GUI/PropertyQPen.h"

class QTN_IMPORT_EXPORT QtnPropertyDelegateQPenStyle
	: public QtnPropertyDelegateTyped<QtnPropertyQPenStyleBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQPenStyle)

public:
	QtnPropertyDelegateQPenStyle(QtnPropertyQPenStyleBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	void applyAttributesImpl(const QtnPropertyDelegateInfo &info) override;

	void drawValueImpl(
		QStylePainter &painter, const QRect &rect) const override;

	QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	bool propertyValueToStrImpl(QString &strValue) const override;

private:
	bool m_showNoPen;
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQPen
	: public QtnPropertyDelegateTypedEx<QtnPropertyQPenBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQPen)

public:
	QtnPropertyDelegateQPen(QtnPropertyQPenBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	void applyAttributesImpl(const QtnPropertyDelegateInfo &info) override;

	QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	bool propertyValueToStrImpl(QString &strValue) const override;
};

#endif // PROPERTY_DELEGATE_QPEN_H
