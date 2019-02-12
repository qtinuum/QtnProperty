/*******************************************************************************
Copyright 2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "QtnProperty/Delegates/PropertyDelegate.h"

#include <QMetaEnum>

class QtnPropertyDelegateFactory;
class QtnPropertyDelegateMetaEnum : public QtnPropertyDelegate
{
	Q_DISABLE_COPY(QtnPropertyDelegateMetaEnum)

	QMetaEnum mMetaEnum;
	bool mShouldTranslate;
	class EditorHandler;

public:
	explicit QtnPropertyDelegateMetaEnum(const QMetaEnum &metaEnum,
		QtnProperty *property, bool translate = false);

	static void Register(QMetaEnum metaEnum,
		QtnPropertyDelegateFactory *factory, bool translate = false);

	template <typename T>
	static void Register(
		QtnPropertyDelegateFactory *factory, bool translate = false)
	{
		Register(QMetaEnum::fromType<T>(), factory, translate);
	}

	static QtnPropertyDelegateInfo delegateInfo(const QMetaEnum &metaEnum);
	template <typename T>
	static inline QtnPropertyDelegateInfo delegateInfo()
	{
		return delegateInfo(QMetaEnum::fromType<T>());
	}

	static QByteArray delegateName(const QMetaEnum &metaEnum);
	template <typename T>
	static inline QByteArray delegateName()
	{
		return delegateName(QMetaEnum::fromType<T>());
	}

	int currentValue() const;

	virtual bool propertyValueToStr(QString &strValue) const override;

	QString valueToStr(int value) const;
	QString keyToStr(const char *key) const;

	static QByteArray translateAttribute();

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateAttributes &attributes) override;
};
