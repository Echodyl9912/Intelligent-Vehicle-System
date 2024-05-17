#include "httpdownload.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HttpDownLoad w;
    w.show();

    return a.exec();
}
