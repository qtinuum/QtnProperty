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

#ifndef PROPERTY_DELEGATE_QSTRING_H
#define PROPERTY_DELEGATE_QSTRING_H

#include "QtnProperty/Delegates/Utils/PropertyDelegateMisc.h"
#include "QtnProperty/Core/PropertyQString.h"

class QTN_IMPORT_EXPORT QtnPropertyDelegateQString
	: public QtnPropertyDelegateTyped<QtnPropertyQStringBase>
{
	Q_DISABLE_COPY(QtnPropertyDelegateQString)

	typedef QtnPropertyDelegateTyped<QtnPropertyQStringBase> Inherited;

public:
	QtnPropertyDelegateQString(QtnPropertyQStringBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;

	virtual bool acceptKeyPressedForInplaceEditImpl(
		QKeyEvent *keyEvent) const override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual bool propertyValueToStrImpl(QString &strValue) const override;

	virtual void drawValueImpl(
		QStylePainter &painter, const QRect &rect) const override;

	bool isPlaceholderColor() const;

protected:
	int m_maxLength;
	bool m_multiline;
	QString m_placeholder;
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQStringInvalidBase
	: public QtnPropertyDelegateQString
{
	Q_DISABLE_COPY(QtnPropertyDelegateQStringInvalidBase)

protected:
	QtnPropertyDelegateQStringInvalidBase(QtnPropertyQStringBase &owner);

	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;
	virtual void drawValueImpl(
		QStylePainter &painter, const QRect &rect) const override;

	virtual bool isPropertyValid() const = 0;

private:
	QColor m_invalidColor;
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQStringFile
	: public QtnPropertyDelegateQStringInvalidBase
{
	Q_DISABLE_COPY(QtnPropertyDelegateQStringFile)

public:
	QtnPropertyDelegateQStringFile(QtnPropertyQStringBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;
	virtual bool propertyValueToStrImpl(QString &strValue) const override;
	virtual bool toolTipImpl(QString &strValue) const override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

	virtual bool isPropertyValid() const override;
	QString defaultDirectory() const;
	QString absoluteFilePath() const;
	QString relativeFilePath() const;
	bool shouldShowRelativePath() const;

private:
	QtnPropertyDelegateInfo m_editorAttributes;
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateQStringList
	: public QtnPropertyDelegateQString
{
	Q_DISABLE_COPY(QtnPropertyDelegateQStringList)

public:
	QtnPropertyDelegateQStringList(QtnPropertyQStringBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;

	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

private:
	QtnPropertyDelegateInfo m_editorAttributes;
};

using QtnGetCandidatesFn = std::function<QStringList()>;
using QtnCreateCandidateFn = std::function<QString(QWidget *, QString)>;
Q_DECLARE_METATYPE(QtnGetCandidatesFn);
Q_DECLARE_METATYPE(QtnCreateCandidateFn);

class QtnPropertyQStringCandidatesComboBoxHandler;

class QTN_IMPORT_EXPORT QtnPropertyDelegateQStringCallback
	: public QtnPropertyDelegateQString
{
	Q_DISABLE_COPY(QtnPropertyDelegateQStringCallback)

public:
	QtnPropertyDelegateQStringCallback(QtnPropertyQStringBase &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	virtual void applyAttributesImpl(
		const QtnPropertyDelegateInfo &info) override;
	virtual QWidget *createValueEditorImpl(QWidget *parent, const QRect &rect,
		QtnInplaceInfo *inplaceInfo = nullptr) override;

private:
	QtnPropertyDelegateInfo m_editorAttributes;
	friend class QtnPropertyQStringCandidatesComboBoxHandler;
};

#endif // PROPERTY_DELEGATE_QSTRING_H
