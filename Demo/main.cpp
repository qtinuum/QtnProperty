#include "MainWindow.h"
#include <QApplication>

void regABColorDelegates();
void regIntListDelegates();
void regLayerDelegates();

int main(int argc, char *argv[])
{
    regABColorDelegates();
    regIntListDelegates();
    regLayerDelegates();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
