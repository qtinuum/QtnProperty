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
						: context.alternateColor());

			};

			subItems.append(bgItem);
		}
	}

	addSubItemBranchNode(context, subItems);
	addSubItemLock(context, subItems);

	// property set name
	{
		QtnSubItem nameItem(context.rect.marginsRemoved(context.margins));
		nameItem.setPropertyDescriptionAsTooltip(m_owner);

		if (nameItem.rect.isValid())
		{
			nameItem.drawHandler = [this](QtnDrawContext &context,
									   const QtnSubItem &item) {
				QFont oldFont = context.painter->font();
				QPen oldPen = context.painter->pen();

				// draw name
				QFont font = oldFont;
				font.setBold(true);
				context.painter->setFont(font);

				context.painter->setPen(
					context.textColorFor(stateProperty()->isEditableByUser()));

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
