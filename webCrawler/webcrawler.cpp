/*
    Author: CoutCin
    Time:2021.1.22-2021.1.24
*/
#include "webcrawler.h"

WebCrawler::WebCrawler(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager,&QNetworkAccessManager::finished,this,&WebCrawler::requestAccessSlot);
    connect(this, SIGNAL(fid()), &loop, SLOT(quit()));      //通过信号停止阻塞循环
    connect(this, SIGNAL(fid2()), &loop2, SLOT(quit()));    //同上
}
//cms列表页请求
void WebCrawler::myRequest(QUrl url)
{
    QNetworkRequest *request = new QNetworkRequest;
    request->setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36");
    request->setUrl(url);
    request->setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    manager->get(*request);
    loop2.exec();//阻塞循环
}
//请求成功后执行的槽函数 通过url判断执行哪个处理
void WebCrawler::requestAccessSlot(QNetworkReply *reply)
{
    qDebug()<<"access!"<<endl;
    if(QString::compare(reply->url().toString(),"https://api.sec.yunsee.cn/api/owner/getVulner"))
    {
        dealWithJson(reply);
        emit fid2();    //处理完发送信号 取消阻塞
    }
    else
    {
        requestIdAccess(reply);
        emit fid();     //处理完发送信号 取消阻塞 不同的处理函数取消不同的阻塞 防止混乱出bug
    }
}
//处理cms详情页面的回复
void WebCrawler::requestIdAccess(QNetworkReply *reply)
{
    QString data = reply->readAll();
    WebCrawler::toString1(data);    //将Unicode转中文
    QJsonParseError json_error;
    Data = QJsonDocument::fromJson(data.toUtf8(), &json_error);     //将Qstring 转为 QJsonDocument
    QJsonArray jA = Data["data"]["data"].toArray();         //定义临时json数组 取data主要数据
    QJsonArray temp;
    //通过for循环将json数组中的每个json对象取出 并将所需数据放到enddata中
    for(int i=0;i<jA.size();i++)
    {
        QJsonValue value = jA[i];
        QJsonObject tt;
        tt.insert("desc_val",value["desc_val"]);
        tt.insert("level",value["level"]);
        tt.insert("vul_id",value["vul_id"]);
        tt.insert("type",value["type"]);
        temp.append(tt);
    }
    QJsonObject t;
    endData->insert("ld",temp);
    emit jsonOk(*endData);//发送信号 让槽函数将这个cms的信息打印出来

//    qDebug()<<*endData<<endl;
    if(nullptr!=endData)//将endData数据释放 并指向空
    {
        delete endData;
        endData = nullptr;
    }

}

//处理cms列表页请求
void WebCrawler::dealWithJson(QNetworkReply *reply)
{
    QString data = reply->readAll();
    WebCrawler::toString1(data);
    QJsonParseError json_error;
    JsonData = QJsonDocument::fromJson(data.toUtf8(), &json_error);

    if(json_error.error == QJsonParseError::NoError)
    {
        QJsonArray jA = JsonData["data"]["data"].toArray();
         //通过for循环将cms列表页中的cms逐个请求，同时将该cms的数据放入endData'中
        for(int i=0;i<jA.size();i++)
        {
            QJsonValue value = jA.at(i);

            endData = new QJsonObject;
            endData->insert("pname",value["pname"][0]);
            endData->insert("domain",value["domain"]);
            endData->insert("company",value["company"]);

            //构造post请求的数据并发送post请求
            QNetworkRequest *req = new QNetworkRequest;
            QJsonObject pData;
            pData.insert("product_id",QString::number(value["id"].toInt()));
            QJsonDocument doc;
            doc.setObject(pData);
            QByteArray postData = doc.toJson();
            req->setUrl(QUrl("https://api.sec.yunsee.cn/api/owner/getVulner"));
            req->setHeader(QNetworkRequest::UserAgentHeader,"user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36");
            req->setHeader(QNetworkRequest::ContentTypeHeader,"application/json;charset=UTF-8");
            manager->post(*req, postData);
            loop.exec();

        }
    }
    else
    {
        qDebug()<<"json error!"<<endl;
    }
}

//将Unicode转中文
void WebCrawler::toString1(QString& buffer)
{
    QRegExp rx("(\\\\u[0-9a-fA-F]{4})");
    int pos = 0;
    while ((pos = rx.indexIn(buffer, pos)) != -1) {
        buffer.replace(pos++, 6, QChar(rx.cap(1).right(4).toUShort(0, 16)));
    }
}


