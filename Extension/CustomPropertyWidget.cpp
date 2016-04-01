#include "CustomPropertyWidget.h"

#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDrag>
#include <QMimeData>

static const QString kDragDropMimeType = "CustomPropertyDragDrop";

CustomPropertyWidget::CustomPropertyWidget(QWidget *parent)
	: QtnPropertyWidgetEx(parent)
	, delegate(nullptr)
{
}

bool CustomPropertyWidget::canRemoveProperty(QtnPropertyBase *property)
{
	return delegate->canRemoveProperty(property);
}

bool CustomPropertyWidget::canCutToClipboard()
{
	return delegate->canCutToClipboard();
}

bool CustomPropertyWidget::canCopyToClipboard()
{
	return (QtnPropertyWidgetEx::canCopyToClipboard()
		&&	delegate->canCopyToClipboard());
}

bool CustomPropertyWidget::canPasteFromClipboard()
{
	return (QtnPropertyWidgetEx::canPasteFromClipboard()
		&&	delegate->canPasteFromClipboard());
}

bool CustomPropertyWidget::dataHasSupportedFormats(const QMimeData *data)
{
	return delegate->dataHasSupportedFormats(data);
}

void CustomPropertyWidget::removeProperty(QtnPropertyBase *property)
{
	delegate->removeProperty(property);
}

QMimeData *CustomPropertyWidget::getPropertyDataForAction(QtnPropertyBase *property,
														  Qt::DropAction action)
{
	return delegate->getPropertyDataForAction(property, action);
}

bool CustomPropertyWidget::applyPropertyData(const QMimeData *data,
											 QtnPropertyBase *destination,
											 QtnApplyPosition position)
{
	return delegate->applyPropertyData(data, destination, position);
}
