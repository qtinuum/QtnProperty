#include "MainWindow.h"
#include <QApplication>

void regABColorDelegates();
void regIntListDelegates();

int main(int argc, char *argv[])
{
    regABColorDelegates();
    regIntListDelegates();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
