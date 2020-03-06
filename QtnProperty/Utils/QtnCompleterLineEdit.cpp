#include "QtnCompleterLineEdit.h"
#include "QtnCompleterItemDelegate.h"

#include <QCompleter>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QListView>
#include <QScrollBar>

enum
{
	POPUP_MARGIN = 6
};

class QtnCompleterLineEdit::ListView : public QListView
{
	unsigned disableHide;

public:
	ListView();

	void beginDisableHide();
	void endDisableHide();
	virtual void setVisible(bool) override;
};

class QtnCompleterLineEdit::Completer : public QCompleter
{
	QtnCompleterLineEdit *mLineEdit;
	ListView *mListView;
	bool mCompleting;

public:
	explicit Completer(QtnCompleterLineEdit *lineEdit);
	virtual ~Completer() override;

	virtual bool eventFilter(QObject *watched, QEvent *event) override;

	void complete();
};

QtnCompleterLineEdit::QtnCompleterLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	mDefaultItemDelegate = new QtnCompleterItemDelegate(this);
	setCompleter(new Completer(this));
}

QtnCompleterLineEdit::~QtnCompleterLineEdit()
{
	setCompleter(nullptr);
	delete mDefaultItemDelegate;
}

QAbstractItemModel *QtnCompleterLineEdit::completerModel() const
{
	return completer()->model();
}

void QtnCompleterLineEdit::setCompleterModel(QAbstractItemModel *model)
{
	auto oldModel = completerModel();
	if (oldModel == model)
		return;

	if (oldModel)
	{
		QObject::disconnect(oldModel, &QAbstractItemModel::modelReset, this,
			&QtnCompleterLineEdit::complete);
		QObject::disconnect(oldModel, &QAbstractItemModel::dataChanged, this,
			&QtnCompleterLineEdit::complete);
		QObject::disconnect(oldModel, &QAbstractItemModel::rowsInserted, this,
			&QtnCompleterLineEdit::complete);
		QObject::disconnect(oldModel, &QAbstractItemModel::rowsRemoved, this,
			&QtnCompleterLineEdit::complete);
	}

	if (model)
	{
		QObject::connect(model, &QAbstractItemModel::modelReset, this,
			&QtnCompleterLineEdit::complete);
		QObject::connect(model, &QAbstractItemModel::dataChanged, this,
			&QtnCompleterLineEdit::complete);
		QObject::connect(model, &QAbstractItemModel::rowsInserted, this,
			&QtnCompleterLineEdit::complete);
		QObject::connect(model, &QAbstractItemModel::rowsRemoved, this,
			&QtnCompleterLineEdit::complete);
	}

	completer()->setModel(model);
	completer()->popup()->setItemDelegate(mDefaultItemDelegate);
}

void QtnCompleterLineEdit::complete()
{
	Q_ASSERT(dynamic_cast<Completer *>(this->completer()));
	auto completer = static_cast<Completer *>(this->completer());
	auto unselectedPrefix = text();
	if (selectionStart() >= 0)
		unselectedPrefix.resize(selectionStart());
	auto model = completer->model();
	bool hasMatch = false;
	for (int i = 0, count = model->rowCount(); i < count; i++)
	{
		if (model->data(model->index(i, 0))
				.toString()
				.contains(unselectedPrefix, Qt::CaseInsensitive))
		{
			hasMatch = true;
			break;
		}
	}
	if (!hasMatch)
		unselectedPrefix.clear();
	if (unselectedPrefix != completer->completionPrefix())
	{
		completer->setCompletionPrefix(unselectedPrefix);
	}
	completer->complete();
}

bool QtnCompleterLineEdit::event(QEvent *e)
{
	return QLineEdit::event(e);
}

QtnCompleterLineEdit::ListView::ListView()
	: disableHide(0)
{
	setUniformItemSizes(true);
	setLayoutMode(Batched);
	setEditTriggers(NoEditTriggers);
	setResizeMode(Adjust);
	setTextElideMode(Qt::ElideNone);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setSelectionBehavior(SelectRows);
	setSelectionMode(SingleSelection);
}

inline void QtnCompleterLineEdit::ListView::beginDisableHide()
{
	disableHide++;
}

inline void QtnCompleterLineEdit::ListView::endDisableHide()
{
	disableHide--;
}

void QtnCompleterLineEdit::ListView::setVisible(bool yes)
{
	if (yes || !disableHide)
		QListView::setVisible(yes);
}

QtnCompleterLineEdit::Completer::Completer(QtnCompleterLineEdit *lineEdit)
	: QCompleter(lineEdit)
	, mLineEdit(nullptr) //do not remove or eventFilter will crash
	, mListView(new ListView)
	, mCompleting(false)
{
	setCompletionMode(QCompleter::PopupCompletion);
	setFilterMode(Qt::MatchContains);
	setWrapAround(true);
	setCaseSensitivity(Qt::CaseInsensitive);
	setPopup(mListView);

	mListView->viewport()->installEventFilter(this);

	mLineEdit = lineEdit;
}

QtnCompleterLineEdit::Completer::~Completer()
{
	mListView->viewport()->removeEventFilter(this);
}

bool QtnCompleterLineEdit::Completer::eventFilter(
	QObject *watched, QEvent *event)
{
	if (!mLineEdit)
		return QCompleter::eventFilter(watched, event);

	bool shouldComplete = false;
	bool disableHide = false;
	bool acceptEvent = false;
	bool escapePressed = false;
	bool finishEdit = false;
	if (!mLineEdit->isReadOnly())
	{
		switch (event->type())
		{
			case QEvent::FocusIn:
				shouldComplete = watched == mLineEdit;
				break;

			case QEvent::MouseButtonPress:
			case QEvent::MouseButtonRelease:
			case QEvent::MouseButtonDblClick:
			case QEvent::MouseMove:
				if (mLineEdit->rect().contains(mLineEdit->mapFromGlobal(
						static_cast<QMouseEvent *>(event)->globalPos())))
				{
					shouldComplete = !mListView->isVisible();
					disableHide = true;
					if (watched != mLineEdit)
					{
						mLineEdit->event(event);
					}
					break;
				}
				if (watched != mLineEdit)
				{
					bool outside = !mListView->isVisible() ||
						!mListView->rect().contains(mListView->mapFromGlobal(
							static_cast<QMouseEvent *>(event)->globalPos()));

					if ((event->type() == QEvent::MouseButtonRelease &&
							!outside) ||
						(event->type() == QEvent::MouseButtonPress && outside))
					{
						acceptEvent = true;
						finishEdit = true;
					}
				}
				break;

			case QEvent::KeyPress:
			{
				auto ke = static_cast<QKeyEvent *>(event);
				switch (ke->key())
				{
					case Qt::Key_Up:
					case Qt::Key_Down:
					case Qt::Key_PageDown:
					case Qt::Key_PageUp:
						break;

					case Qt::Key_Left:
					case Qt::Key_Right:
					case Qt::Key_Home:
					case Qt::Key_End:
						acceptEvent = true;
						shouldComplete = true;
						break;

					case Qt::Key_Tab:
					case Qt::Key_Backtab:
						acceptEvent = true;
						break;

					case Qt::Key_Enter:
					case Qt::Key_Return:
					{
						setCompletionPrefix(mLineEdit->text());
						acceptEvent = true;
						disableHide = true;
						shouldComplete = true;
						break;
					}

					case Qt::Key_Escape:
						escapePressed = true;
						if (watched == mLineEdit ||
							!mListView->selectionModel()->hasSelection())
						{
							break;
						}
						// fallthrough
					default:
					{
						acceptEvent = true;
						disableHide = true;
						shouldComplete = true;
						break;
					}
				}
				break;
			}

			default:
				break;
		}
	}
	if (disableHide)
		mListView->beginDisableHide();

	bool result = QCompleter::eventFilter(watched, event);
	if (acceptEvent && !result && watched != mLineEdit && event->isAccepted())
	{
		watched->event(event);
		result = true;
	}

	if (disableHide)
		mListView->endDisableHide();

	if (finishEdit)
	{
		auto index = currentIndex();
		if (index.isValid())
		{
			emit activated(index);
		} else
		{
			QMetaObject::invokeMethod(
				mLineEdit, "editingFinished", Qt::QueuedConnection);
		}
	} else if (escapePressed)
	{
		if (watched != mLineEdit && mListView->selectionModel()->hasSelection())
		{
			mListView->clearSelection();
		} else
		{
			emit mLineEdit->escaped();
		}
		result = true;
	} else if (shouldComplete)
	{
		mLineEdit->complete();
	}

	return result;
}

void QtnCompleterLineEdit::Completer::complete()
{
	if (mCompleting)
		return;

	mCompleting = true;

	auto popup = this->popup();
	QRect rect(0, 0,
		qMin(popup->maximumWidth(),
			qMax(mLineEdit->width(),
				popup->sizeHintForColumn(0) + POPUP_MARGIN)),
		mLineEdit->height());

	int h = popup->sizeHintForRow(0) *
			qMin(maxVisibleItems(), completionModel()->rowCount()) +
		POPUP_MARGIN;
	popup->setMinimumHeight(h);
	QCompleter::complete(rect);

	QScrollBar *hsb = popup->horizontalScrollBar();
	if (hsb && hsb->isVisible())
		h += popup->horizontalScrollBar()->sizeHint().height();
	if (popup->height() < h)
	{
		popup->setMinimumHeight(h);

		QCompleter::complete(rect);
	}

	mCompleting = false;
}
