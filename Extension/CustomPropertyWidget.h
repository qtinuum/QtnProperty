#pragma once

#include "PropertyWidgetEx.h"

class VarProperty;

struct CustomPropertyData;

class CustomPropertyWidget : public QtnPropertyWidgetEx
{
	Q_OBJECT

public:
	explicit CustomPropertyWidget(QWidget *parent = nullptr);

	inline bool isReadOnly() const;
	void setReadOnly(bool value);
	inline QVariant *getData() const;
	void setData(QVariant *data_ptr, const QString &title = QString(), bool force = false);

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
	void dataEdited();

private slots:
	void onPropertyValueAccept(const QtnProperty *property, void *valueToAccept, bool *accept);

protected:
	virtual void editData();
	virtual bool dataHasSupportedFormats(const QMimeData *data) override;
	virtual void deleteProperty(QtnPropertyBase *property) override;
	virtual QMimeData *getPropertyDataForAction(QtnPropertyBase *property,
												Qt::DropAction dropAction) override;
	virtual bool applyPropertyData(const QMimeData *data,
								   QtnPropertyBase *destination,
								   QtnApplyPosition position) override;

private:
	void updateSet(QtnPropertyBase *set_property, int child_index);

	bool getActiveVarProperty(QtnPropertyBase *&property, VarProperty *&var_property);
	QtnPropertyBase *newProperty(QtnPropertySet *parent,
								 const QVariant &value,
								 const QString &key,
								 int index,
								 VarProperty *map_parent);


	void addProperty(QtnPropertyBase *source, const CustomPropertyData &data);
	void duplicateProperty(QtnPropertyBase *source, const CustomPropertyData &data);
	void updatePropertyOptions(QtnPropertyBase *source, const CustomPropertyData &data);

	QVariant::Type last_add_type;
	QVariant *data_ptr;
	QtnPropertySet *root_set;

	bool read_only;
	bool auto_update;
};

bool CustomPropertyWidget::isReadOnly() const
{
	return read_only;
}

QVariant *CustomPropertyWidget::getData() const
{
	return data_ptr;
}

bool CustomPropertyWidget::isAutoUpdate() const
{
	return auto_update;
}
