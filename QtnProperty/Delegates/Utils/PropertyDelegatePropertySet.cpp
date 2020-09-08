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

#include "PropertyDelegatePropertySet.h"
#include "QtnProperty/Delegates/PropertyDelegateFactory.h"

QtnPropertyDelegatePropertySet::QtnPropertyDelegatePropertySet(
	QtnPropertySet &owner)
	: QtnPropertyDelegate(owner)
	, m_owner(owner)
{
}

void QtnPropertyDelegatePropertySet::Register(
	QtnPropertyDelegateFactory &factory)
{
	factory.registerDelegateDefault(&QtnPropertySet::staticMetaObject,
		&qtnCreateDelegate<QtnPropertyDelegatePropertySet, QtnPropertySet>,
		"Default");
}

int QtnPropertyDelegatePropertySet::subPropertyCountImpl() const
{
	return m_owner.childProperties().size();
}

QtnPropertyBase *QtnPropertyDelegatePropertySet::subPropertyImpl(int index)
{
	return m_owner.childProperties()[index];
}

void QtnPropertyDelegatePropertySet::createSubItemsImpl(
	QtnDrawContext &context, QList<QtnSubItem> &subItems)
{
	// background
	{
		QtnSubItem bgItem(context.rect);

		if (bgItem.rect.isValid())
		{
			bgItem.drawHandler = [](QtnDrawContext &context,
									 const QtnSubItem &item) {
				// fill background
				context.painter->fillRect(item.rect,
					(context.isActive)
						? context.palette().color(QPalette::Highlight)
						: context.palette().color(QPalette::Dark));

			};

			subItems.append(bgItem);
		}
	}

	// branch node sign
	if (context.hasChildren)
	{
		QtnSubItem brItem(context.rect.marginsRemoved(context.margins));
		brItem.rect.setWidth(brItem.rect.height());
		context.margins.setLeft(context.margins.left() + brItem.rect.height());
		brItem.trackState();

		if (brItem.rect.isValid())
		{
			brItem.drawHandler = [this](QtnDrawContext &context,
									 const QtnSubItem &item) {
				auto &painter = *context.painter;
				QRectF branchRect = item.rect;
				qreal side = branchRect.height() / static_cast<qreal>(3.5);
				QColor fillClr = context.palette().color(QPalette::Text);
				QColor outlineClr = (item.state() != QtnSubItemStateNone)
					? Qt::blue
					: context.palette().color(QPalette::Text);

				painter.save();
				painter.setPen(outlineClr);

				QPainterPath branchPath;
				if (propertyImmutable()->stateLocal() &
					QtnPropertyStateCollapsed)
				{
					branchPath.moveTo(
						branchRect.left() + side, branchRect.top() + side);
					branchPath.lineTo(branchRect.right() - side - 1,
						branchRect.top() +
							branchRect.height() / static_cast<qreal>(2.0));
					branchPath.lineTo(
						branchRect.left() + side, branchRect.bottom() - side);
					branchPath.closeSubpath();
				} else
				{
					branchPath.moveTo(
						branchRect.left() + side, branchRect.top() + side);
					branchPath.lineTo(
						branchRect.right() - side, branchRect.top() + side);
					branchPath.lineTo(branchRect.left() +
							branchRect.width() / static_cast<qreal>(2.0),
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

			brItem.eventHandler = [this](QtnEventContext &context,
									  const QtnSubItem &,
									  QtnPropertyToEdit *) -> bool {
				if ((context.eventType() == QEvent::MouseButtonPress) ||
					(context.eventType() == QEvent::MouseButtonDblClick))
				{
					property()->toggleState(QtnPropertyStateCollapsed);
					return true;
				}

				return false;
			};

			subItems.append(brItem);
		}
	}

	addLockItem(context, subItems);

	// property set name
	{
		QtnSubItem nameItem(context.rect.marginsRemoved(context.margins));
		nameItem.setPropertyDescriptionAsTooltip(m_owner);

		if (nameItem.rect.isValid())
		{
			nameItem.drawHandler = [this](QtnDrawContext &context,
									   const QtnSubItem &item) {
				QPalette::ColorGroup cg = stateProperty()->isEditableByUser()
					? context.colorGroup()
					: QPalette::Disabled;

				QFont oldFont = context.painter->font();
				QPen oldPen = context.painter->pen();

				// draw name
				QFont font = oldFont;
				font.setBold(true);
				context.painter->setFont(font);

				context.painter->setPen(context.palette().color(cg,
					(context.isActive) ? QPalette::HighlightedText
									   : QPalette::Text));

				QString elidedName = context.painter->fontMetrics().elidedText(
					property()->displayName(), Qt::ElideRight,
					item.rect.width());
				context.painter->drawText(item.rect,
					Qt::AlignLeading | Qt::AlignVCenter | Qt::TextSingleLine,
					elidedName);

				context.painter->setPen(oldPen);
				context.painter->setFont(oldFont);
			};

			subItems.append(nameItem);
		}
	}
}
