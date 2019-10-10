#include "QtnCompleterItemDelegate.h"

#include "QtnCompleterLineEdit.h"

#include <QPainter>
#include <QCompleter>

QtnCompleterItemDelegate::QtnCompleterItemDelegate(
	QtnCompleterLineEdit *lineEdit, QObject *parent)
	: QStyledItemDelegate(parent)
	, mLineEdit(lineEdit)
{
	Q_ASSERT(mLineEdit);
}

void QtnCompleterItemDelegate::paint(QPainter *painter,
	const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	auto updatedOption = option;
	updatedOption.textElideMode = Qt::ElideNone;
	bool contains = false;
	auto completer = mLineEdit->completer();
	Q_ASSERT(completer);
	switch (completer->filterMode())
	{
		case Qt::MatchStartsWith:
		case Qt::MatchContains:
		case Qt::MatchEndsWith:
		{
			auto subString = completer->completionPrefix();
			auto currentStr = index.data().toString();
			QFontMetrics fm(updatedOption.font);
			auto textRect = mLineEdit->style()->subElementRect(
				QStyle::SE_ItemViewItemText, &updatedOption);
			int flags = Qt::TextSingleLine | int(option.displayAlignment);
			painter->setFont(option.font);
			auto highlightRect = textRect;
			switch (completer->filterMode())
			{
				case Qt::MatchStartsWith:
					if (currentStr.startsWith(
							subString, completer->caseSensitivity()))
					{
						highlightRect.setWidth(fm.width(subString));
						contains = true;
					}
					break;
				case Qt::MatchContains:
				{
					int i = currentStr.indexOf(
						subString, completer->caseSensitivity());
					if (i >= 0)
					{
						highlightRect.setLeft(fm.width(
							QString::fromRawData(currentStr.constData(), i)));
						highlightRect.setWidth(fm.width(subString));
						contains = true;
					}
					break;
				}
				case Qt::MatchEndsWith:
					if (currentStr.endsWith(
							subString, completer->caseSensitivity()))
					{
						highlightRect.setLeft(fm.width(
							QString::fromRawData(currentStr.constData(),
								currentStr.length() - subString.length())));
						contains = true;
					}
					break;
			}
			if (contains)
			{
				painter->save();
				painter->setPen(Qt::transparent);
				painter->setBrush(Qt::yellow);
				painter->drawRect(highlightRect);
				painter->setPen(Qt::white);
				painter->setBrush(Qt::transparent);
				painter->setCompositionMode(
					QPainter::CompositionMode_Difference);
				painter->drawText(textRect, flags, option.text);
				painter->restore();
			}
			break;
		}

		default:
			qWarning("Unsupported filter mode");
			break;
	}

	updatedOption.text.clear();
	QStyledItemDelegate::paint(painter, updatedOption, index);
}
