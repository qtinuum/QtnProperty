/*
   Copyright 2015-2016 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
	return (quint32) val + qtn_u_2;
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
	return QString::number(qtn_i2u(val));
}
