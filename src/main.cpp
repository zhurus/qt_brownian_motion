#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    srand( static_cast<unsigned int>(time(0)) );
    QApplication a(argc, argv);
    Widget w;
//    w.init();
//    w.run();
    w.show();
    return a.exec();
}
