#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "mydialog.h"
#include <QMessageBox>

#include "Demo.peg.h"

using namespace qtn;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pw->setParts(PropertyWidgetPartsDescriptionPanel);

#ifdef Q_OS_WIN32
    ui->pw->propertyView()->setItemHeightRatio(1.3f);
#endif

    ui->pw->setProperty(new PropertySetMain(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_editButton_clicked()
{
    Property* properties = ui->pw->property();

    if (!properties)
    {
        QMessageBox::warning(this, "Demo", "Property is not set.");
        return;
    }

    // copy of properties
    Property* cpy = properties->createCopy(this);
    Q_ASSERT(cpy);
    // defaults
    Property* dft = properties->createNew(this);
    MyDialog dlg(this, *cpy, dft);
    if (dlg.exec() == QMessageBox::Ok)
    {

    }
}
