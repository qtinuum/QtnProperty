/*******************************************************************************
Copyright 2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License")();
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#pragma once

#include "CoreAPI.h"

#include <QByteArray>

QTN_IMPORT_EXPORT QByteArray qtnMultiLineEditAttr();
QTN_IMPORT_EXPORT QByteArray qtnMaxLengthAttr();
QTN_IMPORT_EXPORT QByteArray qtnItemsAttr();
QTN_IMPORT_EXPORT QByteArray qtnEditableAttr();
QTN_IMPORT_EXPORT QByteArray qtnInvalidColorAttr();
QTN_IMPORT_EXPORT QByteArray qtnShowRelativePathAttr();
QTN_IMPORT_EXPORT QByteArray qtnFileModeAttr();
QTN_IMPORT_EXPORT QByteArray qtnFileNameFilterAttr();
QTN_IMPORT_EXPORT QByteArray qtnFileNameFiltersAttr();
QTN_IMPORT_EXPORT QByteArray qtnDefaultDirAttr();
QTN_IMPORT_EXPORT QByteArray qtnOptionsAttr();
QTN_IMPORT_EXPORT QByteArray qtnViewModeAttr();
QTN_IMPORT_EXPORT QByteArray qtnAcceptModeAttr();
QTN_IMPORT_EXPORT QByteArray qtnDefaultSuffixAttr();
QTN_IMPORT_EXPORT QByteArray qtnSuffixAttr();
QTN_IMPORT_EXPORT QByteArray qtnLabelFalseAttr();
QTN_IMPORT_EXPORT QByteArray qtnLabelTrueAttr();
QTN_IMPORT_EXPORT QByteArray qtnShapeAttr();

QTN_IMPORT_EXPORT QByteArray qtnLineEditDelegate();
QTN_IMPORT_EXPORT QByteArray qtnSelectEditDelegate();
QTN_IMPORT_EXPORT QByteArray qtnComboBoxDelegate();
QTN_IMPORT_EXPORT QByteArray qtnCheckBoxDelegate();

struct QtnPropertyDelegateInfo;

QTN_IMPORT_EXPORT QByteArray qtnSpinBoxDelegate();

QTN_IMPORT_EXPORT void qtnInitPercentSpinBoxDelegate(
	QtnPropertyDelegateInfo &delegate);
QTN_IMPORT_EXPORT void qtnInitDegreeSpinBoxDelegate(
	QtnPropertyDelegateInfo &delegate);

QTN_IMPORT_EXPORT double qtnHundredPercent(double value);

template <typename T>
inline T qtnDefaultGet(T value)
{
	return value;
}
