#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "mydialog.h"
#include <QMessageBox>

#include "Demo.peg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pw->setParts(QtnPropertyWidgetPartsDescriptionPanel);

//#ifdef Q_OS_WIN32
    ui->pw->propertyView()->setItemHeightRatio(1.3f);
//#endif

    QtnPropertySet* ps = new QtnPropertySetMain(this);
    ui->pw->setPropertySet(ps);

    qtnScriptRegisterPropertyTypes(&jsEngine);
    jsEngine.globalObject().setProperty("params", jsEngine.newQObject(ps));

    dbg.attachTo(&jsEngine);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_editButton_clicked()
{
    QtnPropertySet* properties = ui->pw->propertySet();

    if (!properties)
    {
        QMessageBox::warning(this, "Demo", "Property is not set.");
        return;
    }

    // copy of properties
    QtnPropertySet* cpy = properties->createCopy(this);
    Q_ASSERT(cpy);
    // defaults
    QtnPropertySet* dft = properties->createNew(this);
    MyDialog dlg(this, *cpy, dft);
    if (dlg.exec() == QDialog::Accepted)
    {
        properties->copyValues(cpy, QtnPropertyStateImmutable|QtnPropertyStateInvisible);
    }
}

void MainWindow::on_dbgButton_clicked()
{
    QMainWindow* dbgWindow = dbg.standardWindow();
    dbgWindow->show();
}

void MainWindow::on_pushButton_clicked()
{
    QtnPropertySet* properties = ui->pw->propertySet();

    if (!properties)
    {
        QMessageBox::warning(this, "Demo", "Property is not set.");
        return;
    }

    QString txt = ui->plainTextEdit->toPlainText();

    if (!properties->fromStr(txt))
    {
        QMessageBox::warning(this, "Demo", "Cannot apply string to Propertyset.");
        return;
    }

    ui->plainTextEdit->clear();
}
