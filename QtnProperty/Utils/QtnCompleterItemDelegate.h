#pragma once

#include <QStyledItemDelegate>

class QtnCompleterLineEdit;
class QtnCompleterItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

	QtnCompleterLineEdit *mLineEdit;

public:
	QtnCompleterItemDelegate(
		QtnCompleterLineEdit *lineEdit, QObject *parent = nullptr);

	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
};
