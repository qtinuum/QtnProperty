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
	updatedOption.text.clear();
	mLineEdit->style()->drawControl(
		QStyle::CE_ItemViewItem, &updatedOption, painter, nullptr);
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
			if (option.displayAlignment & Qt::AlignRight)
			{
				textRect.setRight(textRect.right() -
					mLineEdit->style()->pixelMetric(
						QStyle::PM_FocusFrameHMargin));
			} else
			{
				textRect.setLeft(textRect.left() +
					mLineEdit->style()->pixelMetric(
						QStyle::PM_FocusFrameHMargin));
			}
			int flags = Qt::TextSingleLine | int(option.displayAlignment);
			painter->save();

			bool contains = false;
			auto highlightRect = textRect;
			switch (completer->filterMode())
			{
				case Qt::MatchStartsWith:
					if (currentStr.startsWith(
							subString, completer->caseSensitivity()))
					{
                        highlightRect.setWidth(fm.boundingRect(QString::fromRawData(
                            currentStr.constData(), subString.length())).width());
						contains = true;
					}
					break;
				case Qt::MatchContains:
				{
					int i = currentStr.indexOf(
						subString, 0, completer->caseSensitivity());
					if (i >= 0)
					{
						highlightRect.setLeft(highlightRect.left() +
                            fm.boundingRect(QString::fromRawData(
                                currentStr.constData(), i)).width());
                        highlightRect.setWidth(fm.boundingRect(QString::fromRawData(
                            &currentStr.constData()[i], subString.length())).width());
						contains = true;
					}
					break;
				}
				case Qt::MatchEndsWith:
					if (currentStr.endsWith(
							subString, completer->caseSensitivity()))
					{
						highlightRect.setLeft(highlightRect.left() +
                            fm.boundingRect(
								QString::fromRawData(currentStr.constData(),
                                    currentStr.length() - subString.length())).width());
						contains = true;
					}
					break;
			}
			painter->setCompositionMode(QPainter::CompositionMode_Difference);
			if (contains)
			{
				painter->setPen(Qt::transparent);
				painter->setBrush(Qt::blue);
				painter->drawRect(highlightRect);
			}
			painter->setPen(Qt::white);
			painter->setBrush(Qt::transparent);
			painter->setFont(option.font);
			painter->drawText(textRect, flags, currentStr);
			painter->restore();
			break;
		}

		default:
			qWarning("Unsupported filter mode");
			break;
	}
}
