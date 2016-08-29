#pragma once

#include <QSpinBox>

#include "PropertyWidgetAPI.h"

class QTN_PW_EXPORT QtnUnsignedSpinBox: public QSpinBox
{
public:
	QtnUnsignedSpinBox(QWidget* parent);

	QLineEdit *lineEdit() const;

	static qint32 qtn_u2i(quint32 val);
	static quint32 qtn_i2u(qint32 val);

protected:
	virtual QValidator::State validate(QString &input, int &pos) const override;
	virtual int valueFromText(const QString& text) const override;
	virtual QString textFromValue(int val) const override;
};
