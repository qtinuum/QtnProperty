#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Demo.peg.h"

using namespace qtn;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PropertyWidget *pw = pw;

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
