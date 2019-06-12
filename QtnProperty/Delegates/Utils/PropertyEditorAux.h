/*******************************************************************************
Copyright (c) 2012-2019 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2015-2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#ifndef PROPERTY_EDITOR_AUX_H
#define PROPERTY_EDITOR_AUX_H

#include "QtnProperty/Config.h"
#include "QtnProperty/Delegates/PropertyDelegate.h"

#include <QLineEdit>
#include <QToolButton>
#include <QComboBox>

class QKeyEvent;
class QtnInplaceInfo;

class QTN_IMPORT_EXPORT QtnLineEditBttn : public QWidget
{
public:
	QtnLineEditBttn(QWidget *parent, const QString &bttnText = "...");

	void setTextForProperty(QtnPropertyBase *property, const QString &text);

	QLineEdit *lineEdit;
	QToolButton *toolButton;
};

class QTN_IMPORT_EXPORT QtnPropertyComboBox : public QComboBox
{
	QtnPropertyDelegate *m_delegate;

public:
	explicit QtnPropertyComboBox(
		QtnPropertyDelegate *delegate, QWidget *parent = Q_NULLPTR);

	inline QtnPropertyDelegate *delegate() const;
	inline QtnPropertyBase *property() const;
	inline QtnPropertyBase *stateProperty() const;

private:
	virtual void paintEvent(QPaintEvent *event) override;

protected:
	virtual void customPaint(QPainter &painter, const QRect &rect);
};

QtnPropertyDelegate *QtnPropertyComboBox::delegate() const
{
	return m_delegate;
}

QtnPropertyBase *QtnPropertyComboBox::property() const
{
	return m_delegate->property();
}

QtnPropertyBase *QtnPropertyComboBox::stateProperty() const
{
	return m_delegate->stateProperty();
}

class QTN_IMPORT_EXPORT QtnComboBoxBttn : public QWidget
{
public:
	QtnComboBoxBttn(QWidget *parent, const QString &bttnText = "...");

	QComboBox *comboBox;
	QToolButton *toolButton;
};

enum QtnNumType
{
	NUM_SIGNED_INT,
	NUM_UNSIGNED_INT,
	NUM_FLOAT
};

QTN_IMPORT_EXPORT bool qtnAcceptForLineEdit(QKeyEvent *keyEvent);
QTN_IMPORT_EXPORT bool qtnAcceptForNumEdit(
	QKeyEvent *keyEvent, QtnNumType type);

QTN_IMPORT_EXPORT void qtnInitLineEdit(
	QLineEdit *lineEdit, QtnInplaceInfo *inplaceInfo);
QTN_IMPORT_EXPORT void qtnInitNumEdit(
	QWidget *numEdit, QtnInplaceInfo *inplaceInfo, QtnNumType type);

#endif // PROPERTY_EDITOR_AUX_H
