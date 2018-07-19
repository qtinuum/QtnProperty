/*******************************************************************************
Copyright 2012-2015 Alex Zhondin <qtinuum.team@gmail.com>
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

#include "Property.h"

#include "PropertyConnector.h"

class QtnPropertyDelegateInfoGetter
{
	Q_DISABLE_COPY(QtnPropertyDelegateInfoGetter)

public:
	virtual const QtnPropertyDelegateInfo *delegateInfo() const = 0;

	virtual ~QtnPropertyDelegateInfoGetter() {}

protected:
	QtnPropertyDelegateInfoGetter() {}
};

class QtnPropertyDelegateInfoGetterValue : public QtnPropertyDelegateInfoGetter
{
public:
	QtnPropertyDelegateInfoGetterValue(
		const QtnPropertyDelegateInfo &delegateInfo);

	virtual const QtnPropertyDelegateInfo *delegateInfo() const override;

private:
	QtnPropertyDelegateInfo m_delegateInfo;
};

class QtnPropertyDelegateInfoGetterCallback
	: public QtnPropertyDelegateInfoGetter
{
public:
	QtnPropertyDelegateInfoGetterCallback(
		const QtnProperty::DelegateInfoCallback &callback);

	virtual const QtnPropertyDelegateInfo *delegateInfo() const override;

private:
	QtnProperty::DelegateInfoCallback m_callback;
	mutable QScopedPointer<QtnPropertyDelegateInfo> m_delegateInfo;
};

QtnProperty::QtnProperty(QObject *parent)
	: QtnPropertyBase(parent)
{
}

QtnProperty::~QtnProperty()
{
	// Do not remove! Will be compile errors.
}

const QtnPropertyDelegateInfo *QtnProperty::delegateInfo() const
{
	if (m_delegateInfoGetter.isNull())
		return 0;

	return m_delegateInfoGetter->delegateInfo();
}

void QtnProperty::setDelegateInfo(const QtnPropertyDelegateInfo &delegate)
{
	m_delegateInfoGetter.reset(
		new QtnPropertyDelegateInfoGetterValue(delegate));
}

void QtnProperty::setDelegateInfoCallback(const DelegateInfoCallback &callback)
{
	m_delegateInfoGetter.reset(
		new QtnPropertyDelegateInfoGetterCallback(callback));
}

QtnProperty *QtnProperty::asProperty()
{
	return this;
}

const QtnProperty *QtnProperty::asProperty() const
{
	return this;
}

QtnPropertyDelegateInfoGetterValue::QtnPropertyDelegateInfoGetterValue(
	const QtnPropertyDelegateInfo &delegate)
	: m_delegateInfo(delegate)
{
}

const QtnPropertyDelegateInfo *
QtnPropertyDelegateInfoGetterValue::delegateInfo() const
{
	return &m_delegateInfo;
}

QtnPropertyDelegateInfoGetterCallback::QtnPropertyDelegateInfoGetterCallback(
	const QtnProperty::DelegateInfoCallback &callback)
	: m_callback(callback)
{
}

const QtnPropertyDelegateInfo *
QtnPropertyDelegateInfoGetterCallback::delegateInfo() const
{
	if (m_delegateInfo.isNull())
	{
		m_delegateInfo.reset(new QtnPropertyDelegateInfo(m_callback()));
	}

	return m_delegateInfo.data();
}
