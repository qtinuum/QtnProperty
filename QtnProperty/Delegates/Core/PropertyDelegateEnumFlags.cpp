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

#include "PropertyDelegateEnumFlags.h"
#include "QtnProperty/Core/PropertyBool.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/MultiProperty.h"

#include <QLineEdit>

static QString enumFlagsProperty2Str(const QtnPropertyEnumFlagsBase &property)
{
	QString text;

	auto enumInfo = property.enumInfo();

	if (nullptr == enumInfo)
		return text;

	auto value = property.value();

	if (value == 0)
		return text;

	for (const QtnEnumValueInfo &e : enumInfo->getVector())
	{
		if (value & e.value())
		{
			if (!text.isEmpty())
				text += "|";

			text += e.displayName();
		}
	}

	return text;
}

class QtnPropertyEnumFlagsLineEditHandler
	: public QtnPropertyEditorHandler<QtnPropertyEnumFlagsBase, QLineEdit>
{
public:
	QtnPropertyEnumFlagsLineEditHandler(
		QtnPropertyDelegate *delegate, QLineEdit &editor);

protected:
	virtual void updateEditor() override;
};

QtnPropertyDelegateEnumFlags::QtnPropertyDelegateEnumFlags(
	QtnPropertyEnumFlagsBase &owner)
	: QtnPropertyDelegateTypedEx<QtnPropertyEnumFlagsBase>(owner)
{
	const QtnEnumInfo *enumInfo = owner.enumInfo();

	if (enumInfo)
	{
		for (const QtnEnumValueInfo &e : enumInfo->getVector())
		{
			if (e.state() == QtnEnumValueStateNone)
			{
				QtnEnumValueType enumValue = e.value();

				auto flagProperty = new QtnPropertyBoolCallback;
				flagProperty->setDisplayName(e.displayName());
				flagProperty->setName(e.name());
				flagProperty->setDescription(
					QtnPropertyEnumFlags::getFlagLabelDescription(
						e.displayName(), owner.displayName()));

				flagProperty->setCallbackValueGet(
					[&owner, enumValue]() -> bool {
						return owner.value() & enumValue;
					});
				flagProperty->setCallbackValueSet(
					[&owner, enumValue](bool value) {
						if (value)
							owner.setValue(owner.value() | enumValue);
						else
							owner.setValue(owner.value() & ~enumValue);
					});

				addSubProperty(flagProperty);
			}
		}
	}
}

void QtnPropertyDelegateEnumFlags::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertyEnumFlagsBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateEnumFlags,
			QtnPropertyEnumFlagsBase>,
		QByteArrayLiteral("FlagsList"));
}

QWidget *QtnPropertyDelegateEnumFlags::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setGeometry(rect);

	new QtnPropertyEnumFlagsLineEditHandler(this, *lineEdit);

	if (inplaceInfo)
	{
		lineEdit->selectAll();
	}

	return lineEdit;
}

bool QtnPropertyDelegateEnumFlags::propertyValueToStrImpl(
	QString &strValue) const
{
	strValue = enumFlagsProperty2Str(owner());
	return true;
}

QtnPropertyEnumFlagsLineEditHandler::QtnPropertyEnumFlagsLineEditHandler(
	QtnPropertyDelegate *delegate, QLineEdit &editor)
	: QtnPropertyEditorHandlerType(delegate, editor)
{
	editor.setReadOnly(true);

	updateEditor();
}

void QtnPropertyEnumFlagsLineEditHandler::updateEditor()
{
	if (stateProperty()->isMultiValue())
	{
		editor().clear();
		editor().setPlaceholderText(
			QtnMultiProperty::getMultiValuePlaceholder());
	} else
	{
		editor().setText(enumFlagsProperty2Str(property()));
		editor().setPlaceholderText(QString());
	}
}
