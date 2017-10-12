/*
   Copyright 2017 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#include <QAbstractSpinBox>

class QTN_IMPORT_EXPORT QtnInt64SpinBox : public QAbstractSpinBox
{
	Q_OBJECT
	Q_DISABLE_COPY(QtnInt64SpinBox)

	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
	Q_PROPERTY(QString cleanText READ cleanText)
	Q_PROPERTY(qint64 minimum READ minimum WRITE setMinimum)
	Q_PROPERTY(qint64 maximum READ maximum WRITE setMaximum)
	Q_PROPERTY(qint64 singleStep READ singleStep WRITE setSingleStep)
	Q_PROPERTY(
		qint64 value READ value WRITE setValue NOTIFY valueChanged USER true)
	Q_PROPERTY(
		int displayIntegerBase READ displayIntegerBase
		WRITE setDisplayIntegerBase)
	Q_PROPERTY(bool isSpecialValue READ isSpecialValue)

public:
	explicit QtnInt64SpinBox(QWidget *parent = nullptr);

	inline qint64 value() const;

	inline const QString &prefix() const;
	void setPrefix(const QString &prefix);

	inline const QString &suffix() const;
	void setSuffix(const QString &suffix);

	QString cleanText() const;

	inline qint64 singleStep() const;
	void setSingleStep(qint64 val);

	inline qint64 minimum() const;
	void setMinimum(qint64 min);

	inline qint64 maximum() const;
	void setMaximum(qint64 max);

	void interpretText();

	void setRange(qint64 min, qint64 max);

	inline int displayIntegerBase() const;
	void setDisplayIntegerBase(int base);

	virtual StepEnabled stepEnabled() const override;

	void setLineEdit(QLineEdit *lineEdit);

	bool isSpecialValue() const;

	virtual void clear() override;
	virtual QSize sizeHint() const override;
	virtual QSize minimumSizeHint() const override;

protected:
	virtual QValidator::State validate(QString &input, int &pos) const override;
	virtual qint64 valueFromText(const QString &text) const;
	virtual QString textFromValue(qint64 val) const;
	virtual void fixup(QString &str) const override;
	virtual void stepBy(int steps) override;
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;
	virtual void changeEvent(QEvent *event) override;
	virtual void focusOutEvent(QFocusEvent *event) override;
	virtual void closeEvent(QCloseEvent *event) override;
	virtual void hideEvent(QHideEvent *event) override;

	void updateEdit(bool withGeometry = false, bool withValue = false);

private:
	enum EmitPolicy
	{
		EmitIfChanged,
		AlwaysEmit,
		NeverEmit
	};

	void onTextChanged(const QString &text);
	void onCursorPositionChanged(int oldPos, int newPos);
	void setValue(qint64 value, EmitPolicy ep, bool updateEdit = true);
	void emitSignals();
	void interpret(EmitPolicy ep);

	qint64 validateAndInterpret(
		QString &input, int *pos, QValidator::State *statePtr) const;

	QSize calcSize(const QString &fixedContent, int h) const;

public slots:
	void setValue(qint64 val);

signals:
	void valueChanged(qint64);
	void valueChanged(const QString &);

private:
	class Validator;
	friend class Validator;

	Validator *mValidator;
	qint64 mValue;
	qint64 mSingleStep;
	qint64 mMinimum;
	qint64 mMaximum;
	int mDisplayIntegerBase;
	bool mPendingEmit;
	bool mCleared;
	bool mIgnoreCursorPosition;
	mutable QSize mCachedSizeHint;
	mutable QSize mCachedMinimumSizeHint;
	QString mPrefix;
	QString mSuffix;
};

qint64 QtnInt64SpinBox::value() const
{
	return mValue;
}

const QString &QtnInt64SpinBox::prefix() const
{
	return mPrefix;
}

const QString &QtnInt64SpinBox::suffix() const
{
	return mSuffix;
}

qint64 QtnInt64SpinBox::singleStep() const
{
	return mSingleStep;
}

qint64 QtnInt64SpinBox::minimum() const
{
	return mMinimum;
}

qint64 QtnInt64SpinBox::maximum() const
{
	return mMaximum;
}

int QtnInt64SpinBox::displayIntegerBase() const
{
	return mDisplayIntegerBase;
}
