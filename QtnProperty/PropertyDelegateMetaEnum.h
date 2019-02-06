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
		const QtnPropertyDelegateAttributes &attributes);
};
