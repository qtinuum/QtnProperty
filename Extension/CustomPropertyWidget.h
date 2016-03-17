#pragma once

#include "PropertyWidgetEx.h"

class CustomPropertyWidget : public QtnPropertyWidgetEx
{
	Q_OBJECT

public:
	explicit CustomPropertyWidget(QWidget *parent = nullptr);

//protected:
//	virtual void mousePressEvent(QMouseEvent *event) override;
//	virtual void moseMoveEvent(QMouseEvent *event) override;
//	virtual void dragEnterEvent(QDragEnterEvent *event) override;
//	virtual void dropEvent(QDragEnterEvent *event) override;
};
