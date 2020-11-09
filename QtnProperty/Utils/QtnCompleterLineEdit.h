#pragma once

#include "QtnProperty/Config.h"

#include <QLineEdit>

class QAbstractItemModel;
class QtnCompleterItemDelegate;

class QTN_IMPORT_EXPORT QtnCompleterLineEdit : public QLineEdit
{
	Q_OBJECT

	class ListView;
	class Completer;
	QtnCompleterItemDelegate *mDefaultItemDelegate;

public:
	explicit QtnCompleterLineEdit(QWidget *parent = nullptr);
	virtual ~QtnCompleterLineEdit() override;

	QAbstractItemModel *completerModel() const;
	void setCompleterModel(QAbstractItemModel *model);

public slots:
	void complete();

signals:
	void escaped();

protected:
	virtual bool event(QEvent *e) override;

private:
	using QLineEdit::setCompleter;
};
