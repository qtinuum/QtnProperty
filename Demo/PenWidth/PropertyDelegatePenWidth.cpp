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

#include "PropertyDelegatePenWidth.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"
#include "PropertyPenWidth.h"
#include "QtnProperty/Delegates/Utils/PropertyEditorHandler.h"

#include <QComboBox>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QPaintEvent>

void regPenWidthDelegates()
{
	QtnPropertyDelegateFactory::staticInstance().registerDelegateDefault(
		&QtnPropertyPenWidthBase::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegatePenWidth,
			QtnPropertyPenWidthBase>);
}

static void drawPenWidth(PenWidth penWidth, QPainter &painter, QRect rect)
{
	rect.adjust(2, 2, -2, -2);

	QPen pen = painter.pen();

	switch (penWidth)
	{
		case PenWidth::Default:
			qtnDrawValueText("Default", painter, rect);
			return;
		case PenWidth::Thin:
			pen.setWidth(1);
			break;
		case PenWidth::Middle:
			pen.setWidth(2);
			break;
		case PenWidth::Thick:
			pen.setWidth(3);
			break;
	}

	painter.save();
	painter.setPen(pen);
	auto midY = rect.center().y();
	painter.drawLine(rect.left(), midY, rect.right(), midY);
	painter.restore();
}

class QtnPropertyPenWidthComboBox : public QComboBox
{
public:
	explicit QtnPropertyPenWidthComboBox(
		QtnPropertyDelegatePenWidth *delegate, QWidget *parent = Q_NULLPTR)
		: QComboBox(parent)
		, m_delegate(delegate)
		, m_property(
			  *static_cast<QtnPropertyPenWidthBase *>(delegate->property()))
		, m_updating(0)
	{
		setLineEdit(nullptr);
		setItemDelegate(new ItemDelegate());

		addItem("", QVariant::fromValue(PenWidth::Default));
		addItem("", QVariant::fromValue(PenWidth::Thin));
		addItem("", QVariant::fromValue(PenWidth::Middle));
		addItem("", QVariant::fromValue(PenWidth::Thick));

		updateEditor();

		QObject::connect(&m_property, &QtnPropertyBase::propertyDidChange, this,
			&QtnPropertyPenWidthComboBox::onPropertyDidChange,
			Qt::QueuedConnection);
		QObject::connect(this,
			static_cast<void (QComboBox::*)(int)>(
				&QComboBox::currentIndexChanged),
			this, &QtnPropertyPenWidthComboBox::onCurrentIndexChanged);
	}

protected:
	void paintEvent(QPaintEvent *event) override
	{
		QComboBox::paintEvent(event);

		QPainter painter(this);

		auto index = currentIndex();
		if (index != -1)
		{
			auto penWidth = currentData().value<PenWidth>();
			drawPenWidth(penWidth, painter,
				event->rect().adjusted(0, 0, -event->rect().height(), 0));
		}
	}

	void updateEditor()
	{
		m_updating++;
		setEnabled(m_property.isEditableByUser());
		for (int i = 0; i < count(); ++i)
		{
			if (itemData(i).value<PenWidth>() == m_property.value())
				setCurrentIndex(i);
		}
		m_updating--;
	}

	void onPropertyDidChange(QtnPropertyChangeReason reason)
	{
		if ((reason & QtnPropertyChangeReasonValue))
			updateEditor();
	}

	void onCurrentIndexChanged(int index)
	{
		if (index >= 0)
		{
			m_property.setValue(
				itemData(index).value<PenWidth>(), m_delegate->editReason());
		}
	}

private:
	QtnPropertyDelegatePenWidth *m_delegate;
	QtnPropertyPenWidthBase &m_property;
	unsigned m_updating;

	class ItemDelegate : public QStyledItemDelegate
	{
	public:
		ItemDelegate() {}

		void paint(QPainter *painter, const QStyleOptionViewItem &option,
			const QModelIndex &index) const override
		{
			QStyledItemDelegate::paint(painter, option, index);
			drawPenWidth(index.data(Qt::UserRole).value<PenWidth>(), *painter,
				option.rect);
		}
	};
};

bool QtnPropertyDelegatePenWidth::propertyValueToStrImpl(
	QString &strValue) const
{
	return owner().toStr(strValue);
}

void QtnPropertyDelegatePenWidth::drawValueImpl(
	QStylePainter &painter, const QRect &rect) const
{
	drawPenWidth(owner().value(), painter, rect);
}

QWidget *QtnPropertyDelegatePenWidth::createValueEditorImpl(
	QWidget *parent, const QRect &rect, QtnInplaceInfo *inplaceInfo)
{
	auto combo = new QtnPropertyPenWidthComboBox(this, parent);
	combo->setGeometry(rect);
	if (inplaceInfo && stateProperty()->isEditableByUser())
		combo->showPopup();

	return combo;
}
