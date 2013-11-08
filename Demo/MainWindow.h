#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtScript/QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>

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
    bool event(QEvent* e) override;
    
private slots:
    void on_editButton_clicked();
    void on_dbgButton_clicked();

private:
    Ui::MainWindow *ui;
    //QJSEngine jsEngine;
    QScriptEngine jsEngine;
    QScriptEngineDebugger dbg;
    QMainWindow* dbgWindow;
};

#endif // MAINWINDOW_H
