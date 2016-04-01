#pragma once

#include "PropertyWidgetEx.h"

class CustomPropertyWidget : public QtnPropertyWidgetEx
{
	Q_OBJECT

public:
	explicit CustomPropertyWidget(QWidget *parent = nullptr);

	void setDelegate(QtnPropertyWidgetExDelegate *delegate);

	virtual bool canRemoveProperty(QtnPropertyBase *property) override;
	virtual bool canCutToClipboard() override;
	virtual bool canCopyToClipboard() override;
	virtual bool canPasteFromClipboard() override;

protected:
	virtual bool dataHasSupportedFormats(const QMimeData *data) override;
	virtual void removeProperty(QtnPropertyBase *property) override;
	virtual QMimeData *getPropertyDataForAction(QtnPropertyBase *property,
												Qt::DropAction action) override;
	virtual bool applyPropertyData(const QMimeData *data,
								   QtnPropertyBase *destination,
								   QtnApplyPosition position) override;

private:
	QtnPropertyWidgetExDelegate *delegate;
};
