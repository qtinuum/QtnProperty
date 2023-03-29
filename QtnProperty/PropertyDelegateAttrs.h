/*******************************************************************************
Copyright 2017-2020 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include "Config.h"

#include <QByteArray>

QTN_IMPORT_EXPORT QByteArray qtnPrecisionAttr();
QTN_IMPORT_EXPORT QByteArray qtnMinAttr();
QTN_IMPORT_EXPORT QByteArray qtnMaxAttr();
QTN_IMPORT_EXPORT QByteArray qtnStepAttr();
QTN_IMPORT_EXPORT QByteArray qtnMultiplierAttr();
QTN_IMPORT_EXPORT QByteArray qtnMultiLineEditAttr();
QTN_IMPORT_EXPORT QByteArray qtnMaxLengthAttr();
QTN_IMPORT_EXPORT QByteArray qtnPlaceholderAttr();
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
QTN_IMPORT_EXPORT QByteArray qtnGetCandidatesFnAttr();
QTN_IMPORT_EXPORT QByteArray qtnCreateCandidateFnAttr();
QTN_IMPORT_EXPORT QByteArray qtnCreateCandidateIconAttr();
QTN_IMPORT_EXPORT QByteArray qtnCreateCandidateToolTipAttr();
QTN_IMPORT_EXPORT QByteArray qtnLabelFalseAttr();
QTN_IMPORT_EXPORT QByteArray qtnLabelTrueAttr();
QTN_IMPORT_EXPORT QByteArray qtnShapeAttr();
QTN_IMPORT_EXPORT QByteArray qtnRgbSubItemsAttr();
QTN_IMPORT_EXPORT QByteArray qtnShowAlphaChannelAttr();

QTN_IMPORT_EXPORT QByteArray qtnFillColorAttr();
QTN_IMPORT_EXPORT QByteArray qtnLiveUpdateAttr();
QTN_IMPORT_EXPORT QByteArray qtnDrawBorderAttr();
QTN_IMPORT_EXPORT QByteArray qtnUpdateByScrollAttr();
QTN_IMPORT_EXPORT QByteArray qtnAnimateAttr();
QTN_IMPORT_EXPORT QByteArray qtnToolTipAttr();

QTN_IMPORT_EXPORT QByteArray qtnXDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnXDescriptionAttr();
QTN_IMPORT_EXPORT QByteArray qtnYDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnYDescriptionAttr();
QTN_IMPORT_EXPORT QByteArray qtnZDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnZDescriptionAttr();

QTN_IMPORT_EXPORT QByteArray qtnLeftDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnLeftDescriptionAttr();
QTN_IMPORT_EXPORT QByteArray qtnTopDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnTopDescriptionAttr();
QTN_IMPORT_EXPORT QByteArray qtnRightDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnRightDescriptionAttr();
QTN_IMPORT_EXPORT QByteArray qtnBottomDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnBottomDescriptionAttr();
QTN_IMPORT_EXPORT QByteArray qtnWidthDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnWidthDescriptionAttr();
QTN_IMPORT_EXPORT QByteArray qtnHeightDisplayNameAttr();
QTN_IMPORT_EXPORT QByteArray qtnHeightDescriptionAttr();
QTN_IMPORT_EXPORT QByteArray qtnCoordinateModeAttr();
QTN_IMPORT_EXPORT QByteArray qtnFieldDelegateNameAttr();

QTN_IMPORT_EXPORT QByteArray qtnLineEditDelegate();
QTN_IMPORT_EXPORT QByteArray qtnSelectFileDelegate();
QTN_IMPORT_EXPORT QByteArray qtnSelectFontDelegate();
QTN_IMPORT_EXPORT QByteArray qtnSelectColorDelegateName();
QTN_IMPORT_EXPORT QByteArray qtnCallbackDelegate();
QTN_IMPORT_EXPORT QByteArray qtnComboBoxDelegate();
QTN_IMPORT_EXPORT QByteArray qtnCheckBoxDelegate();
QTN_IMPORT_EXPORT QByteArray qtnLTWHDelegateName();
QTN_IMPORT_EXPORT QByteArray qtnLTRBDelegateName();
QTN_IMPORT_EXPORT QByteArray qtnGeoCoordDelegateName();
QTN_IMPORT_EXPORT QByteArray qtnGeoPointDelegateName();
QTN_IMPORT_EXPORT QByteArray qtnSliderBoxDelegate();
QTN_IMPORT_EXPORT QByteArray qtnSpinBoxDelegate();
QTN_IMPORT_EXPORT QByteArray qtnSolidDelegateName();
QTN_IMPORT_EXPORT QByteArray qtnLinkDelegateName();

QTN_IMPORT_EXPORT QByteArray qtnShowAllAttr();
QTN_IMPORT_EXPORT QByteArray qtnShowNoPenAttr();
QTN_IMPORT_EXPORT QByteArray qtnEditColorAttr();
QTN_IMPORT_EXPORT QByteArray qtnEditStyleAttr();
QTN_IMPORT_EXPORT QByteArray qtnEditCapStyleAttr();
QTN_IMPORT_EXPORT QByteArray qtnEditJoinStyleAttr();
QTN_IMPORT_EXPORT QByteArray qtnEditWidthAttr();
QTN_IMPORT_EXPORT QByteArray qtnTitleAttr();

QTN_IMPORT_EXPORT QByteArray qtnTranslateAttribute();

struct QtnPropertyDelegateInfo;

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
