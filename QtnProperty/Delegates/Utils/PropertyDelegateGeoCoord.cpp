/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
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

#include "PropertyDelegateGeoCoord.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"
#include "QtnProperty/PropertyDelegateAttrs.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorAux.h"

#include <QLineEdit>

QByteArray qtnGeoCoordDelegateName()
{
	return QByteArrayLiteral("GeoCoord");
}

void QtnPropertyDelegateGeoCoord::Register(QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegate(&QtnPropertyDoubleBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegateGeoCoord, QtnPropertyDoubleBase>,
		qtnGeoCoordDelegateName());
}

QString val2strGeoCoord(const double c)
{
	QString s;
	s = c < 0 ? "-" : "";
	double r = qAbs(c);
	int deg = static_cast<int>(r);
	r -= deg;
	r *= 60;
	int min = static_cast<int>(r);
	r -= min;
	r *= 60;
	int sec = static_cast<int>(r);
	if (sec == 60)
	{
		sec = 0;
		++min;
		if (min == 60)
		{
			min = 0;
			++deg;
		}
	}

	QString txt = QString::fromUtf8("%1 %2° %3\' %4\"")
					  .arg(s)
					  .arg(deg, 3, 10, QChar('0'))
					  .arg(min, 2, 10, QChar('0'))
					  .arg(sec, 2, 10, QChar('0'));
	return txt;
}

double str2valGeoCoord(const QString &strVal)
{
	static const QRegExp parserDeg(
		QString::fromUtf8(".*(\\d+)°.*"), Qt::CaseInsensitive);
	static const QRegExp parserMin(".*(\\d+)\'.*", Qt::CaseInsensitive);
	static const QRegExp parserSec(".*(\\d+\\.?\\d*)\".*", Qt::CaseInsensitive);
	static const QRegExp parserSign("^(-).*", Qt::CaseInsensitive);

	QString str = strVal;
	str.remove(" ");
	qreal val = 0.;
	if (parserDeg.exactMatch(str))
	{
		if (parserDeg.capturedTexts().size() == 2)
		{
			val += parserDeg.capturedTexts().at(1).toInt();
		}
	}
	if (parserMin.exactMatch(str))
	{
		if (parserMin.capturedTexts().size() == 2)
		{
			val += parserMin.capturedTexts().at(1).toInt() / 60.;
		}
	}
	if (parserSec.exactMatch(str))
	{
		if (parserSec.capturedTexts().size() == 2)
		{
			val += parserSec.capturedTexts().at(1).toDouble() / 60. / 60.;
		}
	}
	if (parserSign.exactMatch(str))
	{
		val = -val;
	}
	return val;
}

class QtnPropertyGeoCoordLineEditHandler
	: public QtnPropertyEditorHandlerVT<QtnPropertyDoubleBase, QLineEdit>
{
public:
	QtnPropertyGeoCoordLineEditHandler(
		QtnPropertyDelegate *delegate, QLineEdit &editor);

private:
	virtual void updateEditor() override;
	void onValueChanged();
};

QtnPropertyDelegateGeoCoord::QtnPropertyDelegateGeoCoord(
	QtnPropertyDoubleBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyDoubleBase>(owner)
{
}

QWidget *QtnPropertyDelegateGeoCoord::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	bool editable = stateProperty()->isEditableByUser();
	auto lineEdit =
		createValueEditorLineEdit(parent, rect, !editable, inplaceInfo);

	new QtnPropertyGeoCoordLineEditHandler(this, *lineEdit);

	if (editable)
		qtnInitNumEdit(lineEdit, inplaceInfo, NUM_SIGNED_INT);

	return lineEdit;
}

bool QtnPropertyDelegateGeoCoord::propertyValueToStrImpl(
	QString &strValue) const
{
	strValue = val2strGeoCoord(owner().value());
	return true;
}

QtnPropertyGeoCoordLineEditHandler::QtnPropertyGeoCoordLineEditHandler(
	QtnPropertyDelegate *delegate, QLineEdit &editor)
	: QtnPropertyEditorHandlerVT(delegate, editor)
{
	editor.setInputMask(QString::fromUtf8("# 000° 00\' 00\""));

	updateEditor();

	editor.installEventFilter(this);
	QObject::connect(&editor, &QLineEdit::editingFinished, this,
		&QtnPropertyGeoCoordLineEditHandler::onValueChanged);
}

void QtnPropertyGeoCoordLineEditHandler::updateEditor()
{
	updating++;

	editor().setEnabled(stateProperty()->isEditableByUser());
	if (stateProperty()->isMultiValue())
	{
		editor().clear();
	} else
	{
		editor().setText(val2strGeoCoord(property().value()));
		editor().selectAll();
	}

	updating--;
}

void QtnPropertyGeoCoordLineEditHandler::onValueChanged()
{
	if (canApply())
	{
		property().setValue(
			str2valGeoCoord(editor().text()), delegate()->editReason());
	}

	applyReset();
}
