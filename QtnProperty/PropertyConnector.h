#pragma once

#include "CoreAPI.h"

#include <QObject>
#include <QMetaProperty>

class QtnPropertyBase;
class QtnPropertySet;

class QTN_IMPORT_EXPORT QtnPropertyConnector : public QObject
{
	Q_OBJECT

public:
	explicit QtnPropertyConnector(QtnPropertyBase *parent);

	void connectProperty(QObject *object, const QMetaProperty &metaProperty);

	bool isResettablePropertyValue() const;
	void resetPropertyValue(bool edit);

	inline QObject *getObject() const;
	inline const QMetaProperty &getMetaProperty() const;

private slots:
	void onValueChanged();
	void onModifiedSetChanged();

private:
	QObject *object;
	QMetaProperty metaProperty;
};

QObject *QtnPropertyConnector::getObject() const
{
	return object;
}

const QMetaProperty &QtnPropertyConnector::getMetaProperty() const
{
	return metaProperty;
}

