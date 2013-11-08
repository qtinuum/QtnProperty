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

    qtn::PropertySet* ps = new PropertySetMain(this);
    ui->pw->setPropertySet(ps);

    jsEngine.globalObject().setProperty("params", jsEngine.newQObject(ps));

    dbg.attachTo(&jsEngine);
    dbgWindow = dbg.standardWindow();
    //dbgWindow->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event(QEvent* e)
{
    if (e->type() == QEvent::Hide)
    {
        delete dbgWindow;
        dbgWindow = nullptr;
    }

    return QMainWindow::event(e);
}

void MainWindow::on_editButton_clicked()
{
    PropertySet* properties = ui->pw->propertySet();

    if (!properties)
    {
        QMessageBox::warning(this, "Demo", "Property is not set.");
        return;
    }

    // copy of properties
    PropertySet* cpy = properties->createCopy(this);
    Q_ASSERT(cpy);
    // defaults
    PropertySet* dft = properties->createNew(this);
    MyDialog dlg(this, *cpy, dft);
    if (dlg.exec() == QDialog::Accepted)
    {
        properties->copyValues(cpy, PropertyStateImmutable|PropertyStateInvisible);
    }
}

void MainWindow::on_dbgButton_clicked()
{
    dbgWindow->show();
}
