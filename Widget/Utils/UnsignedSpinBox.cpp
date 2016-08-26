#include "UnsignedSpinBox.h"

#include <limits>

static const quint32 qtn_u_2 = std::numeric_limits<quint32>::max() / 2 + 1;

QtnUnsignedSpinBox::QtnUnsignedSpinBox(QWidget *parent)
	: QSpinBox(parent)
{
}

QLineEdit *QtnUnsignedSpinBox::lineEdit() const
{
	return QSpinBox::lineEdit();
}

qint32 QtnUnsignedSpinBox::qtn_u2i(quint32 val)
{
	return qint32(val - qtn_u_2);
}

quint32 QtnUnsignedSpinBox::qtn_i2u(qint32 val)
{
	return (quint32)val + qtn_u_2;
}

int QtnUnsignedSpinBox::valueFromText(const QString &text) const
{
	return qtn_u2i(locale().toUInt(text));
}

QString QtnUnsignedSpinBox::textFromValue(int val) const
{
	return locale().toString(qtn_i2u(val)).replace(QRegExp("\\s*"), "");
}
