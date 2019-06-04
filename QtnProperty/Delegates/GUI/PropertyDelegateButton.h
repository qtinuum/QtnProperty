/*******************************************************************************
Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>
Copyright (c) 2019 Alexandra Cherdantseva <neluhus.vagus@gmail.com>

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

#ifndef PROPERTY_DELEGATE_BUTTON_H
#define PROPERTY_DELEGATE_BUTTON_H

#include "QtnProperty/Delegates/Utils/PropertyDelegateMisc.h"
#include "QtnProperty/GUI/PropertyButton.h"

class QtnPropertyButton;

class QTN_IMPORT_EXPORT QtnPropertyDelegateButton
	: public QtnPropertyDelegateTyped<QtnPropertyButton, QtnPropertyDelegate>
{
	Q_DISABLE_COPY(QtnPropertyDelegateButton)

public:
	QtnPropertyDelegateButton(QtnPropertyButton &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	void applyAttributesImpl(const QtnPropertyDelegateInfo &info) override;
	void createSubItemsImpl(
		QtnDrawContext &context, QList<QtnSubItem> &subItems) override;

	QString m_title;
};

class QTN_IMPORT_EXPORT QtnPropertyDelegateButtonLink
	: public QtnPropertyDelegateButton
{
	Q_DISABLE_COPY(QtnPropertyDelegateButtonLink)

public:
	QtnPropertyDelegateButtonLink(QtnPropertyButton &owner);

	static void Register(QtnPropertyDelegateFactory &factory);

protected:
	void createSubItemsImpl(
		QtnDrawContext &context, QList<QtnSubItem> &subItems) override;

private:
	QCursor m_widgetCursor;
};

#endif // PROPERTY_DELEGATE_BUTTON_H
