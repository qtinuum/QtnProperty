/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#ifndef PROPERTY_DELEGATE_QCOLOR_H
#define PROPERTY_DELEGATE_QCOLOR_H

#include "QtnProperty/Delegates/Utils/PropertyDelegateMisc.h"
#include "QtnProperty/GUI/PropertyQColor.h"

class QTN_IMPORT_EXPORT QtnPropertyDelegateQColor
	: public QtnPropertyDelegateTypedEx<QtnPropertyQColorBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQColor)

public:
	QtnPropertyDelegateQColor(QtnPropertyQColorBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;

	virtual void drawValueImpl(
		QStylePainter &painter, const QRect &rect) const override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual bool propertyValueToStrImpl(QString &strValue) const override;

private:
	quint32 m_shape;
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQColorSolid
	: public QtnPropertyDelegateTyped<QtnPropertyQColorBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQColorSolid)

public:
	QtnPropertyDelegateQColorSolid(QtnPropertyQColorBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual bool createSubItemValueImpl(
		QtnDrawContext &context, QtnSubItem &subItemValue) override;

	virtual void drawValueImpl(
		QStylePainter &painter, const QRect &rect) const override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;
};

#endif // PROPERTY_DELEGATE_QCOLOR_H
