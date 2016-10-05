#pragma once

#include "PropertyWidgetEx.h"

class VarProperty;

struct CustomPropertyData;

class QTN_IMPORT_EXPORT CustomPropertyWidget : public QtnPropertyWidgetEx
{
	Q_OBJECT

public:
	explicit CustomPropertyWidget(QWidget *parent = nullptr);

	inline bool isReadOnly() const;
	void setReadOnly(bool value);
	inline QVariant *getData() const;
	void setData(QVariant *dataPtr, const QString &title = QString(), bool force = false);

	virtual bool canDeleteProperty(QtnPropertyBase *property) override;
	virtual bool canCutToClipboard() override;

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
	void onPropertyValueAccept(const QtnProperty *property, void *valueToAccept, bool *accept);

protected:
	virtual void editData(const QVariant &oldValue);
	virtual bool dataHasSupportedFormats(const QMimeData *data) override;
	virtual void deleteProperty(QtnPropertyBase *property) override;
	virtual QMimeData *getPropertyDataForAction(QtnPropertyBase *property,
												Qt::DropAction dropAction) override;
	virtual bool applyPropertyData(const QMimeData *data,
								   QtnPropertyBase *destination,
								   QtnApplyPosition position) override;

private:
	void updateSet(QtnPropertyBase *setProperty, int childIndex);

	bool getActiveVarProperty(QtnPropertyBase *&property, VarProperty *&varProperty);
	QtnPropertyBase *newProperty(QtnPropertySet *parent,
								 const QVariant &value,
								 const QString &key,
								 int index,
								 VarProperty *mapParent);


	void addProperty(QtnPropertyBase *source, const CustomPropertyData &data);
	void duplicateProperty(QtnPropertyBase *source, const CustomPropertyData &data);
	void updatePropertyOptions(QtnPropertyBase *source, const CustomPropertyData &data);

	QVariant::Type lastAddType;
	QVariant *dataPtr;
	QtnPropertySet *rootSet;

	bool readOnly;
	bool autoUpdate;
};

bool CustomPropertyWidget::isReadOnly() const
{
	return readOnly;
}

QVariant *CustomPropertyWidget::getData() const
{
	return dataPtr;
}

bool CustomPropertyWidget::isAutoUpdate() const
{
	return autoUpdate;
}
