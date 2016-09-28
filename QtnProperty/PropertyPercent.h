#pragma once

#include "Auxiliary/PropertyTemplates.h"
#include "Core/PropertyDouble.h"
#include "Delegates/Core/PropertyDelegateDouble.h"

class QTN_IMPORT_EXPORT QtnPropertyPercentBase
	: public QtnPropertyDoubleBase
{
	Q_OBJECT

public:
	explicit QtnPropertyPercentBase(QObject *parent);

protected:
	virtual bool fromStrImpl(const QString& str, bool edit) override;
	virtual bool toStrImpl(QString& str) const override;
};

class QTN_IMPORT_EXPORT QtnPropertyPercent
	: public QtnSinglePropertyValue<QtnPropertyPercentBase>
{
	Q_OBJECT
	QtnPropertyPercent(const QtnPropertyPercent& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyPercent(QObject *parent);

	static void Register();

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyPercent, QtnPropertyPercentBase)
};

class QTN_IMPORT_EXPORT QtnPropertyPercentCallback
	: public QtnSinglePropertyCallback<QtnPropertyPercentBase>
{
	Q_OBJECT
	QtnPropertyPercentCallback(const QtnPropertyPercentCallback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyPercentCallback(QObject *parent);

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyPercentCallback, QtnPropertyPercentBase)
};

class QTN_IMPORT_EXPORT QtnPropertyDelegatePercent
	: public QtnPropertyDelegateDouble
{
public:
	QtnPropertyDelegatePercent(QtnPropertyPercentBase& owner);
};
