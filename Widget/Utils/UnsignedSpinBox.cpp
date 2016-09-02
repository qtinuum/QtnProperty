#include "UnsignedSpinBox.h"

#include <limits>

static const quint32 qtn_u_2 = std::numeric_limits<quint32>::max() / 2 + 1;

QtnUnsignedSpinBox::QtnUnsignedSpinBox(QWidget *parent)
	: QSpinBox(parent)
{
	setRange(qtn_u2i(0), qtn_u2i(0xFFFFFFFF));
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

QValidator::State QtnUnsignedSpinBox::validate(QString &input, int &) const
{
	bool ok = false;
	auto value = locale().toUInt(input, &ok);
	if (!ok)
		return QValidator::Invalid;
	if (value < qtn_i2u(minimum()))
		return QValidator::Intermediate;
	if (value > qtn_i2u(maximum()))
		return QValidator::Intermediate;

	return QValidator::Acceptable;

}

int QtnUnsignedSpinBox::valueFromText(const QString &text) const
{
	return qtn_u2i(locale().toUInt(text));
}

QString QtnUnsignedSpinBox::textFromValue(int val) const
{
	return locale().toString(qtn_i2u(val)).replace(QRegExp("\\s*"), "");
}
