#include "mydialog.h"
#include "ui_mydialog.h"

using namespace qtn;

MyDialog::MyDialog(QWidget *parent, Property &property, const Property *defaultProperty) :
    QDialog(parent),
    ui(new Ui::MyDialog),
    m_defaultProperty(defaultProperty)
{
    ui->setupUi(this);

    ui->widget->setProperty(&property);
}

MyDialog::~MyDialog()
{
    delete ui;
}
