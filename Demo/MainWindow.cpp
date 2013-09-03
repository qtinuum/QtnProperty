#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PropertyWidget.h"

#include "Demo.peg.h"

using namespace qtn;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PropertyWidget *pw(new PropertyWidget(this));
    setCentralWidget(pw);

    pw->setParts(PropertyWidgetPartsDescriptionPanel);

#ifdef Q_OS_WIN32
    pw->propertyView()->setItemHeightRatio(1.3f);
#endif

    pw->setProperty(new PropertySetMain(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
