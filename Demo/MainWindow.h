#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtQml/QJSEngine>

#include "Property.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    
private slots:
    void on_editButton_clicked();
    void on_commitButton_clicked();
    void on_scriptCode_textChanged();

private:
    void commitCode();

    Ui::MainWindow *ui;
    QJSEngine jsEngine;
};

#endif // MAINWINDOW_H
