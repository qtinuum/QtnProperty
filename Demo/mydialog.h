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
    MyDialog(QWidget* parent, qtn::PropertySet& properties, const qtn::PropertySet* defaultProperties = 0);
    ~MyDialog();
    
private:
    Ui::MyDialog *ui;
    const qtn::PropertySet* m_defaultProperty;
};

#endif // MYDIALOG_H
