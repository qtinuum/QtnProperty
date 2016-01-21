#include "MainWindow.h"
#include <QApplication>

void regABColorDelegates();

int main(int argc, char *argv[])
{
    regABColorDelegates();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
