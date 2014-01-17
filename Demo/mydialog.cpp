#include "mydialog.h"
#include "ui_mydialog.h"

MyDialog::MyDialog(QWidget* parent, QtnPropertySet& propertySet, const QtnPropertySet* defaultProperty) :
    QDialog(parent),
    ui(new Ui::MyDialog),
    m_defaultProperty(defaultProperty)
{
    ui->setupUi(this);

    ui->widget->setPropertySet(&propertySet);
}

MyDialog::~MyDialog()
{
    delete ui;
}
