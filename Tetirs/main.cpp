#include "mainwindow.h"

#include <QApplication>//包含一个应用程序类

int main(int argc, char *argv[])//argc命令行变量的数量 argv命令行变量的数组
{
    QApplication a(argc, argv);// QApplication的对象 有且仅有一个
    MainWindow w;//窗口对象
    w.show();//显示方法 show 是以顶层的方式显示出窗口或控件
    //让程序进入消息循环
    return a.exec();//
}
