#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*
    Author: CoutCin
    Time:2021.1.22-2021.1.24
*/

#include <QMainWindow>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QSslSocket>
#include "webcrawler.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    friend class WebCrawler;
private slots:
    void on_pushButton_clicked();
    void toListW(QJsonObject value);
private:
    WebCrawler crawler;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
