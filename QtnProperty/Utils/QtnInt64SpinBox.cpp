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

#include "QtnInt64SpinBox.h"

#include <QLineEdit>
#include <QStyleOptionSpinBox>
#include <QApplication>
#include <QKeyEvent>

class QtnInt64SpinBox::Validator : public QValidator
{
public:
	explicit Validator(QtnInt64SpinBox *parent);

	virtual State validate(QString &input, int &pos) const override;
	virtual void fixup(QString &input) const override;

private:
	QtnInt64SpinBox *mParent;
};

QtnInt64SpinBox::QtnInt64SpinBox(QWidget *parent)
	: QAbstractSpinBox(parent)
	, mValidator(new Validator(this))
	, mValue(0)
	, mSingleStep(1)
	, mMinimum(std::numeric_limits<qint64>::min())
	, mMaximum(std::numeric_limits<qint64>::max())
	, mDisplayIntegerBase(10)
	, mPendingEmit(false)
	, mCleared(false)
	, mIgnoreCursorPosition(false)
{
	setInputMethodHints(Qt::ImhDigitsOnly);
	auto edit = new QLineEdit(this);
	edit->setObjectName(QLatin1String("qt_spinbox_lineedit"));
	setLineEdit(edit);
}

void QtnInt64SpinBox::setPrefix(const QString &prefix)
{
	if (mPrefix != prefix)
	{
		mPrefix = prefix;

		updateEdit(true);
	}
}

void QtnInt64SpinBox::setSuffix(const QString &suffix)
{
	if (mSuffix != suffix)
	{
		mSuffix = suffix;

		updateEdit(true);
	}
}

QString QtnInt64SpinBox::cleanText() const
{
	return textFromValue(mValue);
}

void QtnInt64SpinBox::setSingleStep(qint64 val)
{
	mSingleStep = val;
}

void QtnInt64SpinBox::setMinimum(qint64 min)
{
	setRange(min, mMaximum);
}

void QtnInt64SpinBox::setMaximum(qint64 max)
{
	setRange(mMinimum, max);
}

void QtnInt64SpinBox::setSpecialValueText(const QString &txt)
{
	if (specialValueText() != txt)
	{
		QAbstractSpinBox::setSpecialValueText(txt);
		updateEdit(true);
	}
}

void QtnInt64SpinBox::interpretText()
{
	interpret(EmitIfChanged);
}

void QtnInt64SpinBox::setRange(qint64 min, qint64 max)
{
	if (min > max)
	{
		qSwap(min, max);
	}

	if (mMinimum == min && mMaximum == max)
	{
		return;
	}

	updateEdit(true, true);
}

void QtnInt64SpinBox::setDisplayIntegerBase(int base)
{
	// Falls back to base 10 on invalid bases (like QString)
	if (Q_UNLIKELY(base < 2 || base > 36))
	{
		qWarning(
			"QtnInt64SpinBox::setDisplayIntegerBase: Invalid base (%d)", base);
		base = 10;
	}

	if (mDisplayIntegerBase != base)
	{
		mDisplayIntegerBase = base;

		updateEdit(true);
	}
}

QAbstractSpinBox::StepEnabled QtnInt64SpinBox::stepEnabled() const
{
	if (isReadOnly())
		return StepNone;

	if (wrapping())
		return StepDownEnabled | StepUpEnabled;

	StepEnabled result;

	if (mValue > mMinimum)
		result |= StepDownEnabled;

	if (mValue < mMaximum)
		result |= StepUpEnabled;

	return result;
}

void QtnInt64SpinBox::setLineEdit(QLineEdit *lineEdit)
{
	Q_ASSERT(nullptr != lineEdit);

	if (lineEdit == this->lineEdit())
	{
		return;
	}

	if (lineEdit)
	{
		connect(
			lineEdit, &QLineEdit::textChanged,
			this, &QtnInt64SpinBox::onTextChanged);
		connect(
			lineEdit, &QLineEdit::cursorPositionChanged,
			this, &QtnInt64SpinBox::onCursorPositionChanged);
	}

	QAbstractSpinBox::setLineEdit(lineEdit);

	lineEdit->setValidator(mValidator);
	updateEdit();
}

bool QtnInt64SpinBox::isSpecialValue() const
{
	return mValue == mMinimum && !specialValueText().isEmpty();
}

void QtnInt64SpinBox::clear()
{
	QAbstractSpinBox::clear();

	lineEdit()->setText(mPrefix + mSuffix);
	lineEdit()->setCursorPosition(mPrefix.size());

	mCleared = true;
}

QSize QtnInt64SpinBox::sizeHint() const
{
	if (mCachedSizeHint.isEmpty())
	{
		mCachedSizeHint = calcSize(
				mPrefix + mSuffix + QLatin1Char(' '),
				lineEdit()->sizeHint().height());
	}

	return mCachedSizeHint;
}

QSize QtnInt64SpinBox::minimumSizeHint() const
{
	if (mCachedMinimumSizeHint.isEmpty())
	{
		mCachedMinimumSizeHint = calcSize(
				mPrefix + QLatin1Char(' '),
				lineEdit()->minimumSizeHint().height());
	}

	return mCachedMinimumSizeHint;
}

qint64 QtnInt64SpinBox::validateAndInterpret(
	QString &input, int *pos, QValidator::State *statePtr) const
{
	int inputSize = input.size();

	auto copy = input.trimmed();

	if (copy.startsWith(mPrefix))
		copy.remove(0, mPrefix.size());

	if (copy.endsWith(mSuffix))
		copy.resize(copy.size() - mSuffix.size());

	auto locale = this->locale();

	QValidator::State state = QValidator::Acceptable;
	qint64 num = mMinimum;

	if (mMinimum != mMaximum && (
			copy.isEmpty() ||
			(mMinimum < 0 && copy == QLatin1String("-")) ||
			(mMaximum >= 0 && copy == QLatin1String("+"))))
	{
		state = QValidator::Intermediate;
	} else
	if (copy.startsWith(QLatin1Char('-')) && mMinimum >= 0)
	{
		state = QValidator::Invalid;
	} else
	{
		bool ok = false;

		if (mDisplayIntegerBase != 10)
		{
			num = copy.toLongLong(&ok, mDisplayIntegerBase);
		} else
		{
			num = locale.toLongLong(copy, &ok);

			if (!ok && copy.contains(locale.groupSeparator()) && (
					mMaximum >= 1000 || mMinimum <= -1000))
			{
				QString copy2 = copy;
				copy2.remove(locale.groupSeparator());
				num = locale.toLongLong(copy2, &ok);
			}
		}

		if (!ok)
		{
			state = QValidator::Invalid;
		} else
		if (num >= mMinimum && num <= mMaximum)
		{
			state = QValidator::Acceptable;
		} else
		if (mMinimum == mMaximum)
		{
			state = QValidator::Invalid;
		} else
		{
			if ((num >= 0 && num > mMaximum) ||
				(num < 0 && num < mMinimum))
			{
				state = QValidator::Invalid;
			} else
			{
				state = QValidator::Intermediate;
			}
		}
	}

	if (state != QValidator::Acceptable)
		num = mMaximum > 0 ? mMinimum : mMaximum;

	input = mPrefix + copy + mSuffix;

	if (pos)
	{
		(*pos) -= inputSize - input.size();
	}

	if (statePtr)
	{
		*statePtr = state;
	}

	return num;
}

QSize QtnInt64SpinBox::calcSize(const QString &fixedContent, int h) const
{
	//Use the prefix and range to calculate the minimumSizeHint
	ensurePolished();

	const QFontMetrics fm(fontMetrics());
	int w = 0;

	QString s;
	s = textFromValue(mMinimum);
	s.truncate(18);
	s += fixedContent;
	w = qMax(w, fm.width(s));
	s = textFromValue(mMaximum);
	s.truncate(18);
	s += fixedContent;
	w = qMax(w, fm.width(s));

	s = specialValueText();

	if (!s.isEmpty())
	{
		w = qMax(w, fm.width(s));
	}

	w += 2;	// cursor blinking space

	QStyleOptionSpinBox opt;
	initStyleOption(&opt);
	QSize hint(w, h);

	return style()->sizeFromContents(
		QStyle::CT_SpinBox, &opt, hint, this)
		   .expandedTo(QApplication::globalStrut());
}

QValidator::State QtnInt64SpinBox::validate(QString &input, int &pos) const
{
	QValidator::State state;
	validateAndInterpret(input, &pos, &state);
	return state;
}

qint64 QtnInt64SpinBox::valueFromText(const QString &text) const
{
	QString copy = text;
	return validateAndInterpret(copy, nullptr, nullptr);
}

QString QtnInt64SpinBox::textFromValue(qint64 val) const
{
	QString str;

	if (mDisplayIntegerBase != 10)
	{
		const QLatin1String prefix = val < 0
			? QLatin1String("-")
			: QLatin1String();
		str = prefix + QString::number(
				qAbs(val), mDisplayIntegerBase);
	} else
	{
		str = locale().toString(val);

		if (!isGroupSeparatorShown())
			str.remove(locale().groupSeparator());
	}

	return str;
}

void QtnInt64SpinBox::fixup(QString &str) const
{
	if (!isGroupSeparatorShown())
		str.remove(locale().groupSeparator());
}

void QtnInt64SpinBox::stepBy(int steps)
{
	auto old = mValue;

	QString input = lineEdit()->text();
	int pos = lineEdit()->cursorPosition();

	bool dontStep = false;
	EmitPolicy ep = EmitIfChanged;

	if (mPendingEmit)
	{
		dontStep = validate(input, pos) != QValidator::Acceptable;

		mCleared = false;
		interpret(NeverEmit);

		if (mValue != old)
		{
			ep = AlwaysEmit;
		}
	}

	if (!dontStep)
	{
		qint64 step = mSingleStep * steps;

		qint64 newValue = mValue + step;

		if (step < 0)
		{
			if (newValue < mMinimum || newValue > mValue)
				newValue = mMinimum;
		} else
		if (step > 0)
		{
			if (newValue > mMaximum || newValue < mValue)
				newValue = mMaximum;
		}

		setValue(newValue, ep);
	} else
	if (ep == AlwaysEmit)
	{
		emitSignals();
	}

	selectAll();
}

void QtnInt64SpinBox::keyPressEvent(QKeyEvent *event)
{
	if (!event->text().isEmpty() &&
		lineEdit()->cursorPosition() < mPrefix.size())
	{
		lineEdit()->setCursorPosition(mPrefix.size());
	}

	QAbstractSpinBox::keyPressEvent(event);

	switch (event->key())
	{
		case Qt::Key_Enter:
		case Qt::Key_Return:
			interpret(keyboardTracking() ? AlwaysEmit : EmitIfChanged);
			selectAll();
			break;

		default:
			break;
	}

	if (mCleared && !lineEdit()->text().isEmpty())
	{
		mCleared = false;
	}
}

void QtnInt64SpinBox::showEvent(QShowEvent *event)
{
	QAbstractSpinBox::showEvent(event);

	updateEdit();
}

void QtnInt64SpinBox::changeEvent(QEvent *event)
{
	QAbstractSpinBox::changeEvent(event);

	if (event->type() == QEvent::ActivationChange)
	{
		if (!isActiveWindow())
		{
			if (mPendingEmit)
				interpret(EmitIfChanged);
		}
	}
}

void QtnInt64SpinBox::focusOutEvent(QFocusEvent *event)
{
	QAbstractSpinBox::focusOutEvent(event);

	if (mPendingEmit)
		interpret(EmitIfChanged);

	updateEdit();
}

void QtnInt64SpinBox::closeEvent(QCloseEvent *event)
{
	QAbstractSpinBox::closeEvent(event);

	if (mPendingEmit)
		interpret(EmitIfChanged);
}

void QtnInt64SpinBox::hideEvent(QHideEvent *event)
{
	QAbstractSpinBox::hideEvent(event);

	if (mPendingEmit)
		interpret(EmitIfChanged);
}

void QtnInt64SpinBox::updateEdit(bool withGeometry, bool withValue)
{
	if (withValue && !isSpecialValue())
	{
		setValue(mValue);
	} else
	if (!mCleared)
	{
		auto edit = lineEdit();

		bool isSpecialValue = this->isSpecialValue();

		const QString newText = isSpecialValue
			? specialValueText()
			: mPrefix + textFromValue(mValue) + mSuffix;

		if (newText == edit->text())
			return;

		const bool empty = edit->text().isEmpty();
		int cursor = edit->cursorPosition();
		int selectedSize = edit->selectedText().size();
		const QSignalBlocker blocker(edit);
		edit->setText(newText);

		if (!isSpecialValue)
		{
			cursor = qBound(
					mPrefix.size(), cursor,
					edit->text().size() - mSuffix.size());

			if (selectedSize > 0)
			{
				edit->setSelection(cursor, selectedSize);
			} else
			{
				edit->setCursorPosition(empty ? mPrefix.size() : cursor);
			}
		}

		update();
	}

	if (withGeometry)
	{
		mCachedMinimumSizeHint = QSize();
		mCachedSizeHint = QSize();
		updateGeometry();
	}
}

void QtnInt64SpinBox::onTextChanged(const QString &text)
{
	mPendingEmit = true;

	if (keyboardTracking())
	{
		QString input = text;
		int pos = lineEdit()->cursorPosition();
		QValidator::State state = validate(input, pos);

		if (state == QValidator::Acceptable)
		{
			setValue(valueFromText(input));
			mPendingEmit = false;
		}
	}
}

void QtnInt64SpinBox::onCursorPositionChanged(int oldPos, int newPos)
{
	auto edit = lineEdit();

	if (!edit->hasSelectedText() && !mIgnoreCursorPosition && !isSpecialValue())
	{
		mIgnoreCursorPosition = true;

		bool allowSelection = true;
		int pos = -1;

		if (newPos < mPrefix.size() && newPos != 0)
		{
			if (oldPos == 0)
			{
				allowSelection = false;
				pos = mPrefix.size();
			} else
			{
				pos = oldPos;
			}
		} else
		if (newPos > edit->text().size() - mSuffix.size()
			&& newPos != edit->text().size())
		{
			if (oldPos == edit->text().size())
			{
				pos = edit->text().size() - mSuffix.size();
				allowSelection = false;
			} else
			{
				pos = edit->text().size();
			}
		}

		if (pos != -1)
		{
			const int selSize = edit->selectionStart() >= 0 &&
				allowSelection
				? (edit->selectedText().size() * (newPos < pos ? -1 : 1)) -
				newPos + pos
				: 0;

			const QSignalBlocker blocker(edit);

			if (selSize != 0)
			{
				edit->setSelection(pos - selSize, selSize);
			} else
			{
				edit->setCursorPosition(pos);
			}
		}

		mIgnoreCursorPosition = false;
	}
}

void QtnInt64SpinBox::setValue(
	qint64 value, QtnInt64SpinBox::EmitPolicy ep, bool updateEdit)
{
	Q_ASSERT(value >= mMinimum);
	Q_ASSERT(value <= mMaximum);

	auto old = mValue;
	mValue = value;
	mPendingEmit = false;
	mCleared = false;

	if (updateEdit)
	{
		this->updateEdit();
	} else
	{
		update();
	}

	if (ep == AlwaysEmit || (ep == EmitIfChanged && old != mValue))
	{
		emitSignals();
	}
}

void QtnInt64SpinBox::emitSignals()
{
	mPendingEmit = false;
	emit valueChanged(lineEdit()->text());
	emit valueChanged(mValue);
}

void QtnInt64SpinBox::interpret(EmitPolicy ep)
{
	if (mCleared)
		return;

	qint64 v = 0;
	bool doInterpret = true;
	QString tmp = lineEdit()->text();
	int pos = lineEdit()->cursorPosition();
	const int oldpos = pos;

	if (validate(tmp, pos) != QValidator::Acceptable)
	{
		const QString copy = tmp;
		fixup(tmp);

		doInterpret = tmp != copy &&
			(validate(tmp, pos) == QValidator::Acceptable);

		if (!doInterpret)
		{
			if (correctionMode() == CorrectToNearestValue)
			{
				if (mMinimum < v)
				{
					v = std::min(v, mMaximum);
				} else
				{
					v = mMinimum;
				}
			} else
			{
				v = mValue;
			}
		}
	}

	if (doInterpret)
	{
		v = valueFromText(tmp);
	}

	setValue(v, ep, true);

	if (oldpos != pos)
		lineEdit()->setCursorPosition(pos);
}

void QtnInt64SpinBox::setValue(qint64 val)
{
	if (val < mMinimum)
		val = mMinimum;

	if (val > mMaximum)
		val = mMaximum;

	if (mValue != val)
	{
		setValue(val, EmitIfChanged);
	}
}

QtnInt64SpinBox::Validator::Validator(QtnInt64SpinBox *parent)
	: QValidator(parent)
	, mParent(parent)
{
	setObjectName(QLatin1String("qt_int64_spinboxvalidator"));
}

QValidator::State QtnInt64SpinBox::Validator::validate(
	QString &input, int &pos) const
{
	auto specialText = mParent->specialValueText();

	if (!specialText.isEmpty() && input == specialText)
		return QValidator::Acceptable;

	auto &prefix = mParent->mPrefix;

	if (!prefix.isEmpty() && !input.startsWith(prefix))
	{
		input.prepend(prefix);
		pos += prefix.length();
	}

	auto &suffix = mParent->mSuffix;

	if (!suffix.isEmpty() && !input.endsWith(suffix))
		input.append(suffix);

	return mParent->validate(input, pos);
}

void QtnInt64SpinBox::Validator::fixup(QString &input) const
{
	mParent->fixup(input);
}
