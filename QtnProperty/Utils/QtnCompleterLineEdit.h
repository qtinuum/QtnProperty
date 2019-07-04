#pragma once

#include "QtnProperty/Config.h"

#include <QLineEdit>

class QAbstractItemModel;

class QTN_IMPORT_EXPORT QtnCompleterLineEdit : public QLineEdit
{
	Q_OBJECT

	class ListView;
	class Completer;

public:
	explicit QtnCompleterLineEdit(QWidget *parent = nullptr);

	QAbstractItemModel *completerModel() const;
	void setCompleterModel(QAbstractItemModel *model);

protected:
	virtual bool event(QEvent *e) override;

private:
	using QLineEdit::setCompleter;
};
