#pragma once

#include "QtnProperty/Delegates/PropertyDelegate.h"

#include <QMetaEnum>

class QtnPropertyDelegateFactory;
class QtnPropertyDelegateMetaEnum : public QtnPropertyDelegate
{
	Q_DISABLE_COPY(QtnPropertyDelegateMetaEnum)

	QMetaEnum mMetaEnum;
	class EditorHandler;

public:
	explicit QtnPropertyDelegateMetaEnum(
		const QMetaEnum &metaEnum, QtnProperty *property);

	static void Register(
		const QMetaEnum &metaEnum, QtnPropertyDelegateFactory *factory);

	int currentValue() const;

	virtual bool propertyValueToStr(QString &strValue) const override;

	QString valueToStr(int value) const;
	QString keyToStr(const char *key) const;

protected:
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;
};
