#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include "Property.h"

namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT
    
public:
    MyDialog(QWidget* parent, QtnPropertySet& properties, const QtnPropertySet* defaultProperties = nullptr);
    ~MyDialog();
    
private:
    Ui::MyDialog *ui;
    const QtnPropertySet* m_defaultProperty;
};

#endif // MYDIALOG_H
