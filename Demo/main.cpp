#include "MainWindow.h"
#include <QApplication>

void regABColorDelegates();
void regIntListDelegates();
void regLayerDelegates();
void regFreqDelegates();

int main(int argc, char *argv[])
{
    regABColorDelegates();
    regIntListDelegates();
    regLayerDelegates();
    regFreqDelegates();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
