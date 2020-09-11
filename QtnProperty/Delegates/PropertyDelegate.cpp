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

#include "PropertyDelegate.h"
#include "Utils/QtnConnections.h"
#include "Utils/PropertyEditorHandler.h"
#include "PropertyView.h"

QtnPropertyDelegate::QtnPropertyDelegate(QtnPropertyBase &ownerProperty)
	: m_ownerProperty(&ownerProperty)
	, m_stateProperty(nullptr)
{
}

QtnPropertyDelegate::~QtnPropertyDelegate()
{
	if (m_editorHandler)
		m_editorHandler->cleanup();
}

void QtnPropertyDelegate::init()
{
	// do nothing
}

QtnPropertyChangeReason QtnPropertyDelegate::editReason() const
{
	QtnPropertyChangeReason result = QtnPropertyChangeReasonEdit;
	if (stateProperty()->isMultiValue())
		result |= QtnPropertyChangeReasonMultiEdit;
	return result;
}

void QtnPropertyDelegate::applySubPropertyInfo(
	const QtnPropertyDelegateInfo &info, const QtnSubPropertyInfo &subInfo)
{
	auto p = subProperty(subInfo.id);
	p->setName(subInfo.key);
	info.storeAttributeValue(subInfo.displayNameAttr, p,
		&QtnPropertyBase::displayName, &QtnPropertyBase::setDisplayName);
	info.storeAttributeValue(subInfo.descriptionAttr, p,
		&QtnPropertyBase::description, &QtnPropertyBase::setDescription);
}

int QtnPropertyDelegate::subPropertyCountImpl() const
{
	return 0;
}

QtnPropertyBase *QtnPropertyDelegate::subPropertyImpl(int index)
{
	Q_UNUSED(index);
	return nullptr;
}

void QtnPropertyDelegate::applyAttributesImpl(
	const QtnPropertyDelegateInfo &info)
{
	Q_UNUSED(info);
}

QStyle::State QtnPropertyDelegate::state(
	bool isActive, const QtnSubItem &subItem) const
{
	auto subState = subItem.state();
	QStyle::State state = QStyle::State_Active;
	if (stateProperty()->isEditableByUser())
		state |= QStyle::State_Enabled;
	if (isActive)
	{
		state |= QStyle::State_Selected;
		state |= QStyle::State_HasFocus;
	}

	if (subState == QtnSubItemStateUnderCursor)
		state |= QStyle::State_MouseOver;
	else if (subState == QtnSubItemStatePushed)
		state |= QStyle::State_Sunken;

	return state;
}

void QtnPropertyDelegate::addSubItemLock(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	if (!stateProperty()->isUnlockable())
	{
		return;
	}

	QtnSubItem item(context.rect.marginsRemoved(context.margins));
	item.rect.setWidth(item.rect.height());
	context.margins.setLeft(context.margins.left() + item.rect.height() +
		context.widget->valueLeftMargin());

	item.setTextAsTooltip(stateProperty()->isLocked()
			? QtnPropertyView::tr("Unlock")
			: QtnPropertyView::tr("Lock"));

	item.trackState();

	if (item.rect.isValid())
	{
		item.drawHandler = [this](QtnDrawContext &context,
							   const QtnSubItem &item) {
			drawButton(context, item, QIcon(),
				stateProperty()->isLocked()
					? QString::fromUtf8("\xF0\x9F\x93\x95")
					: QString::fromUtf8("\xF0\x9F\x93\x96"));
		};

		item.eventHandler = [this](QtnEventContext &context, const QtnSubItem &,
								QtnPropertyToEdit *) -> bool {
			if ((context.eventType() == QEvent::MouseButtonPress) ||
				(context.eventType() == QEvent::MouseButtonDblClick))
			{
				QtnConnections connections;
				context.widget->connectPropertyToEdit(
					stateProperty(), connections);
				stateProperty()->toggleLock(editReason());
				return true;
			}

			return false;
		};

		subItems.append(item);
	}
}

QColor QtnPropertyDelegate::disabledTextColor(const QStylePainter &painter)
{
	auto palette = painter.style()->standardPalette();
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
	return palette.color(QPalette::PlaceholderText);
#else
	return palette.color(QPalette::Disabled, QPalette::Text);
#endif
}

void QtnPropertyDelegate::addSubItemBranchNode(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	if (!context.hasChildren)
		return;

	QtnSubItem brItem(context.rect.marginsRemoved(context.margins));
	brItem.rect.setWidth(brItem.rect.height());
	context.margins.setLeft(context.margins.left() + brItem.rect.height());
	brItem.trackState();

	if (!brItem.rect.isValid())
		return;

	brItem.drawHandler = [this](
							 QtnDrawContext &context, const QtnSubItem &item) {
		auto &painter = *context.painter;
		QRectF branchRect = item.rect;
		qreal side = branchRect.height() / qreal(3.5);
		QColor fillClr = context.palette().color(QPalette::Text);
		QColor outlineClr = (item.state() != QtnSubItemStateNone)
			? Qt::blue
			: context.palette().color(QPalette::Text);

		painter.save();
		painter.setPen(outlineClr);

		QPainterPath branchPath;
		if (stateProperty()->isCollapsed())
		{
			branchPath.moveTo(
				branchRect.left() + side, branchRect.top() + side);
			branchPath.lineTo(branchRect.right() - side - 1,
				branchRect.top() + branchRect.height() / qreal(2.0));
			branchPath.lineTo(
				branchRect.left() + side, branchRect.bottom() - side);
			branchPath.closeSubpath();
		} else
		{
			branchPath.moveTo(
				branchRect.left() + side, branchRect.top() + side);
			branchPath.lineTo(
				branchRect.right() - side, branchRect.top() + side);
			branchPath.lineTo(
				branchRect.left() + branchRect.width() / qreal(2.0),
				branchRect.bottom() - side - 1);
			branchPath.closeSubpath();
		}

		if (painter.testRenderHint(QPainter::Antialiasing))
		{
			painter.fillPath(branchPath, fillClr);
			painter.drawPath(branchPath);
		} else
		{
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.fillPath(branchPath, fillClr);
			painter.drawPath(branchPath);
			painter.setRenderHint(QPainter::Antialiasing, false);
		}

		painter.restore();
	};

	brItem.eventHandler = [this](QtnEventContext &context, const QtnSubItem &,
							  QtnPropertyToEdit *) -> bool {
		if ((context.eventType() == QEvent::MouseButtonPress) ||
			(context.eventType() == QEvent::MouseButtonDblClick))
		{
			stateProperty()->toggleState(QtnPropertyStateCollapsed);
			return true;
		}

		return false;
	};

	brItem.tooltipHandler = [this](QtnEventContext &,
								const QtnSubItem &) -> QString {
		return (stateProperty()->isCollapsed())
			? QtnPropertyView::tr("Click to expand")
			: QtnPropertyView::tr("Click to collapse");
	};

	subItems.append(brItem);
}
