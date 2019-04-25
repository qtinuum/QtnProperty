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

private slots:
    void on_editButton_clicked();
    void on_dbgButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    QStringList getCandidates() const { return m_candidates; }
    QString createCandidate(QWidget* parent);

    Ui::MainWindow *ui;

    QScriptEngineDebugger dbg;
    QScriptEngine jsEngine;

    QStringList m_candidates = {"one", "two", "three"};
};

#endif // MAINWINDOW_H
