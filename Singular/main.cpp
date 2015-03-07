#include "singular.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Singular w;
    w.show();

    return a.exec();
}
