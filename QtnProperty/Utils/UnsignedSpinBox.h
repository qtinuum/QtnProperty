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

#pragma once

#include "QtnProperty/CoreAPI.h"

#include <QSpinBox>

class QTN_IMPORT_EXPORT QtnUnsignedSpinBox: public QSpinBox
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
