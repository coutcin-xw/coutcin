#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTableWidgetItem>
/*
    Author: CoutCin
    Time:2021.1.22-2021.1.24
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&crawler,SIGNAL(jsonOk(QJsonObject)),this,SLOT(toListW(QJsonObject)));  //连接json处理信号和函数
}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_pushButton_clicked()
{

    int n=ui->lineEdit_2->text().toInt();//从输入框获取数字
    //进行请求  从第一页一直请求到获取的页面
    for(int i=1;i<=n;i++)
    {
        QUrl url="https://api.sec.yunsee.cn/api/owner/program/lists?page="+QString::number(i);
        crawler.myRequest(url);
    }

}
//将数据显示到页面
void MainWindow::toListW(QJsonObject value)
{
      QTreeWidgetItem *f = new QTreeWidgetItem(ui->treeWidget,QStringList(value["pname"].toString()));
      QTreeWidgetItem *temp = new QTreeWidgetItem(f,QStringList(value["domain"].toString()));
      f->addChild(temp);
      QTreeWidgetItem *temp1 = new QTreeWidgetItem(f,QStringList(value["company"].toString()));
      f->addChild(temp1);
      QJsonArray jA = value["ld"].toArray();
      QTreeWidgetItem *temp2 = new QTreeWidgetItem(f,QStringList("历史漏洞："));
      f->addChild(temp2);
      for(int i=0;i<jA.size();i++)
      {
          QTreeWidgetItem *temp3 = new QTreeWidgetItem(temp2,QStringList(jA.at(i)["vul_id"].toString()+" "+jA.at(i)["level"].toString()+" "+jA.at(i)["type"].toString()));
          temp2->addChild(temp3);

      }

//    qDebug()<<"dd"<<endl;
}
