/*******************************************************************************
Copyright 2015-2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <QDoubleSpinBox>

#include "QtnProperty/CoreAPI.h"

class QTN_IMPORT_EXPORT QtnDoubleSpinBox : public QDoubleSpinBox
{
public:
	explicit QtnDoubleSpinBox(QWidget *parent = 0);

	virtual QString textFromValue(double val) const override;

	static QString valueToText(double value, const QLocale &locale = QLocale(),
		int decimals = 10, bool groupSeparatorShown = false);
};
