#include "PropertyWidgetEx.h"

QtnPropertyWidgetEx::QtnPropertyWidgetEx(QWidget *parent)
	: QtnPropertyWidget(parent)
{
	for (auto child : children())
	{
		auto vbox = dynamic_cast<QVBoxLayout *>(child);

		if (nullptr != vbox)
		{
			vbox->setContentsMargins(0, 0, 0, 0);
			vbox->setMargin(0);
			vbox->setSpacing(0);
		}
	}
}
