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

    ui->scriptCode->installEventFilter(this);

    ui->pw->setParts(PropertyWidgetPartsDescriptionPanel);

#ifdef Q_OS_WIN32
    ui->pw->propertyView()->setItemHeightRatio(1.3f);
#endif

    qtn::PropertySet* ps = new PropertySetMain(this);
    ui->pw->setPropertySet(ps);

    jsEngine.globalObject().setProperty("params", jsEngine.newQObject(ps));

    ui->scriptCode->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == ui->scriptCode)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Return && keyEvent->modifiers() == Qt::ControlModifier)
            {
                commitCode();
                return true;
            }
        }

        return false;
    }
    else
    {
        return QMainWindow::eventFilter(obj, event);
    }
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

void MainWindow::on_commitButton_clicked()
{
    commitCode();
}

void MainWindow::commitCode()
{
    QString code = ui->scriptCode->document()->toPlainText();
    QJSValue result = jsEngine.evaluate(code);

    if (!result.isError())
        ui->scriptCode->clear();

    QString resText = result.toString();
    if (!resText.isEmpty())
    {
        QTextCursor c(ui->scriptOutput->document());
        if (result.isError())
            c.insertHtml(QString("<p><font color='#ff0000'>%1<br></p>").arg(resText));
        else
            c.insertHtml(QString("<p>%1<br></p>").arg(resText));
    }
}

void MainWindow::on_scriptCode_textChanged()
{
    QString code = ui->scriptCode->document()->toPlainText();
    ui->commitButton->setDisabled(code.isEmpty());
}
