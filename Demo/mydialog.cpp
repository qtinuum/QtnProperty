#include "mydialog.h"
#include "ui_mydialog.h"

using namespace qtn;

MyDialog::MyDialog(QWidget* parent, PropertySet& propertySet, const PropertySet* defaultProperty) :
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
