#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "mydialog.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include <QInputDialog>

#include "QObjectPropertySet.h"
#include "Demo.peg.h"

// this is required for QStringCallbackProperty delegate parameters
#include "Delegates/Core/PropertyDelegateQString.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pw->setParts(QtnPropertyWidgetPartsDescriptionPanel);

    auto ps = new QtnPropertySetSamplePS(this);

    ps->LayerProperty.setLayers({{"Top", Qt::red, 0}, {"Left", Qt::blue, 1}, {"Right", Qt::green, 2}, {"Bottom", Qt::gray, 3}});

    ps->QStringCallbackProperty.setDelegate({"Callback"});
    QVariant attr;
    attr.setValue(qtnMemFn(this, &MainWindow::getCandidates));
    ps->QStringCallbackProperty.setDelegateAttribute("GetCandidatesFn", attr);
    attr.setValue(qtnMemFn(this, &MainWindow::createCandidate));
    ps->QStringCallbackProperty.setDelegateAttribute("CreateCandidateFn", attr);
    ps->QStringCallbackProperty.setDelegateAttribute("CreateCandidateLabel", "<add item...>");

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

QString MainWindow::createCandidate(QWidget* parent)
{
    bool ok;
    QString text = QInputDialog::getText(parent, "Create item", "Name:", QLineEdit::Normal, "new", &ok);
    if (ok && !text.isEmpty())
    {
        if (m_candidates.indexOf(text) < 0)
            m_candidates.append(text);
        else
            QMessageBox::warning(parent, "Create item", "Cannot create duplicate.");

        return  text;
    }

    return {};
}

