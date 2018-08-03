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

#pragma once

#include "PropertyWidgetEx.h"

class VarProperty;

struct QtnCustomPropertyData;

class QTN_IMPORT_EXPORT QtnCustomPropertyWidget : public QtnPropertyWidgetEx
{
	Q_OBJECT

public:
	explicit QtnCustomPropertyWidget(QWidget *parent = nullptr);

	inline bool isReadOnly() const;
	void setReadOnly(bool value);
	inline QVariant *getData() const;
	void setData(QVariant *dataPtr, const QString &title = QString(),
		bool force = false);

	virtual bool canDeleteProperty(QtnPropertyBase *property) override;
	virtual bool canCutToClipboard() override;
	virtual bool canPasteFromClipboard() override;

	void addProperty();
	void duplicateProperty();
	void propertyOptions();

	inline bool isAutoUpdate() const;
	void setAutoUpdate(bool yes);
	void updateData();

	static VarProperty *getVarProperty(QtnPropertyBase *source);

signals:
	void dataEdited(const QVariant &oldValue);

private slots:
	void onPropertyValueAccept(void *valueToAccept, bool *accept);

protected:
	virtual void editData(const QVariant &oldValue);
	virtual bool dataHasSupportedFormats(const QMimeData *data) override;
	virtual void deleteProperty(QtnPropertyBase *property) override;
	virtual QMimeData *getPropertyDataForAction(
		QtnPropertyBase *property, Qt::DropAction dropAction) override;
	virtual bool applyPropertyData(const QMimeData *data,
		QtnPropertyBase *destination, QtnApplyPosition position) override;
	virtual void dropEvent(QDropEvent *event) override;
	virtual void dropEnd() override;

private:
	void updateSet(QtnPropertyBase *setProperty, int childIndex);

	bool getActiveVarProperty(
		QtnPropertyBase *&property, VarProperty *&varProperty);
	QtnPropertyBase *newProperty(QtnPropertySet *parent, const QVariant &value,
		const QString &key, int index, VarProperty *mapParent);

	void addProperty(
		QtnPropertyBase *source, const QtnCustomPropertyData &data);
	void duplicateProperty(
		QtnPropertyBase *source, const QtnCustomPropertyData &data);
	void updatePropertyOptions(
		QtnPropertyBase *source, const QtnCustomPropertyData &data);
	bool insertReplaceOrCancel(
		QtnPropertyBase *destination, QtnCustomPropertyData &customData);

	QVariant *dataPtr;
	QVariant::Type lastAddType;
	bool readOnly : 1;
	bool autoUpdate : 1;
	bool backupAutoUpdate : 1;

protected:
	QtnPropertySet *rootSet;
};

bool QtnCustomPropertyWidget::isReadOnly() const
{
	return readOnly;
}

QVariant *QtnCustomPropertyWidget::getData() const
{
	return dataPtr;
}

bool QtnCustomPropertyWidget::isAutoUpdate() const
{
	return autoUpdate;
}
