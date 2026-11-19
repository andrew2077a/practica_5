#include "graficos.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    graficos w;
    w.show();
    return a.exec();
}
