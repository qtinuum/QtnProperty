#pragma once

#include "Property.h"
#include "Delegates/PropertyDelegate.h"

#include <QMetaProperty>

#include <set>
#include <memory>

class QtnMultiPropertyDelegate;
class QTN_IMPORT_EXPORT QtnMultiProperty : public QtnProperty
{
	Q_OBJECT

public:
	explicit QtnMultiProperty(const QMetaObject *propertyMetaObject, QObject *parent = nullptr);
	virtual ~QtnMultiProperty();

	void addProperty(QtnProperty *property, bool own = true);
	void resetValues();

	bool hasResettableValues() const;
	bool hasMultipleValues() const;

	static void Register();
	static QString getMultiValuePlaceholder();

	inline std::vector<QtnProperty *> getProperties() const;

	QMetaProperty getMetaProperty() const;

private slots:
	void onPropertyValueAccept(QtnPropertyValuePtr valueToAccept, bool* accept);

	void onPropertyWillChange(QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue, int typeId);

	void onPropertyDidChange(QtnPropertyChangeReason reason);

protected:
	virtual bool loadImpl(QDataStream &stream) override;
	virtual bool saveImpl(QDataStream &stream) const override;

	virtual bool fromStrImpl(const QString &str, bool edit) override;
	virtual bool toStrImpl(QString &str) const override;

	virtual bool fromVariantImpl(const QVariant& var, bool edit) override;
	virtual bool toVariantImpl(QVariant& var) const override;

private:
	void updateMultipleState(bool force);

	std::vector<QtnProperty *> properties;
	const QMetaObject *propertyMetaObject;

	bool calculateMultipleValues;
	bool multipleValues;

	friend class QtnMultiPropertyDelegate;
};

std::vector<QtnProperty *> QtnMultiProperty::getProperties() const
{
	return properties;
}

class QtnMultiPropertyDelegate
		: public QtnPropertyDelegateTypedEx<QtnMultiProperty>
{
	Q_DISABLE_COPY(QtnMultiPropertyDelegate)

	typedef QtnPropertyDelegateTypedEx<QtnMultiProperty> Inherited;

public:
	QtnMultiPropertyDelegate(QtnMultiProperty& owner);
	virtual ~QtnMultiPropertyDelegate();

private:
	struct PropertyToEdit
	{
		QtnMultiProperty *owner;
		QtnProperty *property;
		std::vector<QMetaObject::Connection> connections;
	};

	static void onEditedPropertyWillChange(PropertyToEdit *data,
										   QtnPropertyChangeReason reason,
										   QtnPropertyValuePtr newValue,
										   int typeId);
	static void onEditedPropertyDidChange(PropertyToEdit *data, QtnPropertyChangeReason reason);
	static void onEditedPropertyDestroyed(PropertyToEdit *data);
	static void onEditorDestroyed(PropertyToEdit *data);

protected:
	virtual bool propertyValueToStr(QString &strValue) const override;

	virtual void applyAttributesImpl(const QtnPropertyDelegateAttributes &attributes) override;
	virtual void drawValueImpl(QStylePainter &painter, const QRect &rect,
							   const QStyle::State &state, bool *needTooltip = nullptr) const override;
	virtual QString toolTipImpl() const override;
	virtual bool acceptKeyPressedForInplaceEditImpl(QKeyEvent *keyEvent) const override;
	virtual QWidget* createValueEditorImpl(QWidget *parent, const QRect &rect,
										   QtnInplaceInfo *inplaceInfo = nullptr) override;

private:
	typedef std::unique_ptr<QtnPropertyDelegate> DelegatePtr;
	std::vector<DelegatePtr> superDelegates;
};

struct QtnMultiVariant
{
	QVariantList values;
};

Q_DECLARE_METATYPE(QtnMultiVariant)

