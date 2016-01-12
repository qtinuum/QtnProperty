#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "mydialog.h"
#include <QMessageBox>
#include <QDesktopWidget>

#include "QObjectPropertySet.h"
#include "Demo.peg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pw->setParts(QtnPropertyWidgetPartsDescriptionPanel);

    QtnPropertySet* ps = new QtnPropertySetSamplePS(this);
    ui->pw->setPropertySet(ps);

    qtnScriptRegisterPropertyTypes(&jsEngine);
    jsEngine.globalObject().setProperty("samplePS", jsEngine.newQObject(ps));

    dbg.attachTo(&jsEngine);

    move(QApplication::desktop()->availableGeometry().center() - rect().center());
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

void MainWindow::on_pushButton_3_clicked()
{
    QtnPropertySet* properties = ui->pw->propertySet();

    if (!properties)
    {
        QMessageBox::warning(this, "Demo", "Property is not set.");
        return;
    }

    QString text;
    if (!properties->toStr(text))
    {
        QMessageBox::warning(this, "Demo", "Cannot get string from Propertyset.");
        return;
    }

    ui->plainTextEdit->setPlainText(text);
}

void MainWindow::on_pushButton_2_clicked()
{
    QtnPropertySet* bttnProperties = qtnCreateQObjectPropertySet(ui->pushButton_2);
    QtnPropertySet* editProperties = qtnCreateQObjectPropertySet(ui->plainTextEdit);
    QtnPropertySet properties(this);

    if (bttnProperties)
        properties.addChildProperty(bttnProperties);

    if (editProperties)
        properties.addChildProperty(editProperties);

    MyDialog dlg(this, properties);
    dlg.exec();
}
