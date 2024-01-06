#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H
/*
    Author: CoutCin
    Time:2021.1.22-2021.1.24
*/
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QQueue>
#include <QRegExp>
#include <QByteArray>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QEventLoop>
#include <QTime>
#include <QNetworkCookie>
class WebCrawler : public QObject
{
    Q_OBJECT
public:

    explicit WebCrawler(QObject *parent = nullptr);
    void myRequest(QUrl url);                           //cms列表页请求

    static void toString1(QString& buffer);             //将Unicode转中文
    void dealWithJson(QNetworkReply *reply);
    void requestIdAccess(QNetworkReply *reply);         //处理cms详情页面的回复
    friend class MainWindow;
signals:
    void jsonOk(QJsonObject value);
    void fid();
    void fid2();
public slots:
    void requestAccessSlot(QNetworkReply *reply);       //请求成功后执行的槽函数 通过url判断执行哪个处理

private:
    QNetworkAccessManager *manager;
    QJsonDocument JsonData;                     //保存cms列表页的jsonDocument
    QJsonDocument Data;                         //保存cms详情页的jsonDocument
    QJsonObject *endData;                       //保存最后获取的json数据
    QTime time;
    QEventLoop loop;
    QEventLoop loop2;
};

#endif // WEBCRAWLER_H
