#include "mainwindow.h"

#include <QApplication>
/*
    Author: CoutCin
    Time:2021.1.22-2021.1.24
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
