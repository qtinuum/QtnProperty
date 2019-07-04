#include "QtnCompleterLineEdit.h"

#include <QCompleter>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QListView>

class QtnCompleterLineEdit::ListView : public QListView
{
public:
	ListView();
	virtual ~ListView() override;
};

class QtnCompleterLineEdit::Completer : public QCompleter
{
	QtnCompleterLineEdit *mLineEdit;

public:
	explicit Completer(QtnCompleterLineEdit *lineEdit);

	virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

QtnCompleterLineEdit::QtnCompleterLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	setCompleter(new Completer(this));
}

QAbstractItemModel *QtnCompleterLineEdit::completerModel() const
{
	return completer()->model();
}

void QtnCompleterLineEdit::setCompleterModel(QAbstractItemModel *model)
{
	completer()->setModel(model);
}

bool QtnCompleterLineEdit::event(QEvent *e)
{
	return QLineEdit::event(e);
}

QtnCompleterLineEdit::ListView::ListView()
{
	setUniformItemSizes(true);
	setLayoutMode(Batched);
	setEditTriggers(NoEditTriggers);
	setResizeMode(Adjust);
	setTextElideMode(Qt::ElideNone);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setSelectionBehavior(SelectRows);
	setSelectionMode(SingleSelection);
}

QtnCompleterLineEdit::ListView::~ListView()
{
	printf("");
}

QtnCompleterLineEdit::Completer::Completer(QtnCompleterLineEdit *lineEdit)
	: QCompleter(lineEdit)
	, mLineEdit(nullptr) //do not remove or eventFilter will crash
{
	setCompletionMode(QCompleter::PopupCompletion);
	setFilterMode(Qt::MatchContains);
	setWrapAround(true);
	setCaseSensitivity(Qt::CaseInsensitive);
	setPopup(new ListView);

	mLineEdit = lineEdit;
}

bool QtnCompleterLineEdit::Completer::eventFilter(
	QObject *watched, QEvent *event)
{
	if (!mLineEdit)
		return QCompleter::eventFilter(watched, event);

	auto popup = this->popup();
	if (watched == mLineEdit || watched == popup)
	{
		switch (event->type())
		{
			case QEvent::FocusIn:
			{
				if (!mLineEdit->isReadOnly() && !popup->isVisible())
				{
					setCompletionPrefix(mLineEdit->text());
					complete();
				}
				break;
			}

			default:
				break;
		}
	}

	bool result = QCompleter::eventFilter(watched, event);

	if (!mLineEdit->isReadOnly() && event->isAccepted() &&
		event->type() == QEvent::KeyPress)
	{
		setCompletionPrefix(mLineEdit->text());
		if (!popup->isVisible())
			complete();
	}

	return result;
}
