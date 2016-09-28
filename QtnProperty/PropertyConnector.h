#pragma once

#include "CoreAPI.h"

#include <QObject>
#include <QMetaProperty>

class QtnProperty;
class QtnPropertySet;

class QTN_IMPORT_EXPORT QtnPropertyConnector : public QObject
{
	Q_OBJECT

public:
	explicit QtnPropertyConnector(QtnProperty *parent);

	void connectProperty(QObject *object, const QMetaProperty &metaProperty);

	bool isResettablePropertyValue() const;
	void resetPropertyValue();

private slots:
	void onValueChanged();

private:
	QObject *object;
	QMetaProperty metaProperty;
};

