#ifndef CLIENT_H
#define CLIENT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<time.h>
#define PORT 7777 //定义端口宏
#define IP "192.168.181.144" //定义服务器ip地址

typedef struct MsgData
{
    int flag;                       //标识
    int work;                       //操作选项
    int account;                    //用户登入账号
    int detAccount;                 //私聊时目标帐号
    char mess[1024];                //消息
    char name[30];                  //用户昵称
    char pass[30];                  //用户密码
    char question[256];             //密保问题
    char answer[256];               //密保答案
    char time[30];                  //时间
} MSG;

int myAccount;
char myName[30];
int clientSock;

void homePage()
{
        printf("\n");
        printf("\t基于多线程的网络聊天室\t\n");
        printf("\t*****************\n");
        printf("\t1.注册\n");
        printf("\t2.登入\n");
        printf("\t3.修改密码\n");
        printf("\t4.修改昵称\n");
        printf("\t5.找回密码\n");
        printf("\t6.退出\n");
        printf("\t群主需抢先注册昵称为root\n");
        printf("\t*****************\n");
}

void registeredAccount(int sock)
{
    printf("\x1b[H\x1b[2J");        //清屏
    printf("*******************注册页面*********************\n");
    int  accountT;                   //用户登入账号
    char nameT[30];                  //用户昵称
    char passT[30];                  //用户密码
    char questionT[256];             //密保问题
    char answerT[256];               //密保答案
    MSG msg;
    memset(&msg,0,sizeof(msg));
    srand((unsigned) time(NULL));				//时间函数生成随机
    accountT = rand()%100000;
    printf("您的帐号为：%d\n",accountT);
    name:
    printf("请输入昵称：(不超过14个字符)\n");
    memset(nameT,0,sizeof(nameT));
    scanf("%s",nameT);
    if(strlen(nameT)==0)
    {
        printf("昵称不能为空！\n");
        goto name;
    }
    if(strlen(passT)>14)
    {
        printf("昵称过长！\n");
        goto name;
    }
    pass:
    printf("请输密码：（密码由数字、字母、下划线组成且不超过16位）\n");
    memset(passT,0,sizeof(passT));
    scanf("%s",passT);
    if(strlen(passT)==0)
    {
        printf("密码不能为空！\n");
        goto pass;
    }
    if(strlen(passT)>16)
    {
        printf("密码过长！\n");
        goto pass;
    }
    question:
    printf("请输密保问题：（不超过50个字）\n");
    memset(questionT,0,sizeof(questionT));
    scanf("%s",questionT);
    if(strlen(questionT)==0)
    {
        printf("密保不能为空！\n");
        goto question;
    }
    if(strlen(questionT)>120)
    {
        printf("字数过长！\n");
        goto question;
    }
    answer:
    printf("请输密保答案：（不超过50个字）\n");
    memset(answerT,0,sizeof(answerT));
    scanf("%s",answerT);
    if(strlen(answerT)==0)
    {
        printf("答案不能为空！\n");
        goto answer;
    }
    if(strlen(answerT)>120)
    {
        printf("字数过长！\n");
        goto answer;
    }
    strcpy(msg.name,nameT);
    strcpy(msg.pass,passT);
    strcpy(msg.question,questionT);
    strcpy(msg.answer,answerT);
    msg.account=accountT;
    msg.work=1;
    send(sock,&msg,sizeof(MSG),0);		//发送给总控制台;
    memset(&msg,0,sizeof(msg));
    if(recv(sock,&msg,sizeof(MSG),0)<=0)//每次都判断
    {
         printf("服务器断开链接\n");
         exit(-1);
    }
    if(msg.flag == 1)
    {
        printf("\x1b[H\x1b[2J");        //清屏
        printf("注册成功\n");
        printf("\n您的登入账号为：%d 请务必牢记\n",msg.account);
    }
    else if(msg.flag == 0)
    {
        printf("服务器错误注册失败\n");
    }
    else if(msg.flag == 3)
    {
        printf("帐号重复请重新注册\n");
    }

}

bool logIn(int sock)
{
    MSG msg;
    msg.work=2;
    int accountT;
    char passT[30];                  //用户密码
    printf("\x1b[H\x1b[2J");        //清屏
    printf("*******************登录页面*********************\n");
    printf("请输帐号：\n");
    scanf("%d",&accountT);
    pass:
    printf("请输密码：（密码由数字、字母、下划线组成且不超过16位）\n");
    memset(passT,0,sizeof(passT));
    scanf("%s",passT);
    if(strlen(passT)==0)
    {
        printf("密码不能为空！\n");
        goto pass;
    }
    if(strlen(passT)>16)
    {
        printf("密码过长！\n");
        goto pass;
    }
    msg.account=accountT;
    strcpy(msg.pass,passT);
    send(sock,&msg,sizeof (MSG),0);
    if(recv(sock,&msg,sizeof(MSG),0) <= 0) //每次都判断
        {
            printf("服务器断开链接\n");
            exit(-1);
        }
        if(msg.flag == 1)
        {
            printf("\x1b[H\x1b[2J");
            printf("\n");
            printf("欢迎你 %s(%d)\n",msg.name,msg.account);
            printf("\n");
            strcpy(myName,msg.name);
            myAccount=msg.account;
        }
        else if(msg.flag == 2)
        {
            printf("该账号未注册！\n");
            return 0;
        }
        else if(msg.flag == 3)
        {
            printf("该帐号已在线！\n");
            return 0;
        }
        else if(msg.flag == 4)
        {
            printf("密码错误！\n");
            return 0;
        }
        else
        {
            printf("系统错误,请重试\n");
            return 0;
        }
    return true;
}

void changePassword(int sock)          //修改密码
{

    MSG msg;
    msg.work=7;
    int accountT;
    char passT[30];                  //用户密码
    printf("\x1b[H\x1b[2J");        //清屏
    printf("*******************修改密码页面*******************\n");
    printf("请输入需要修改帐号：\n");
    scanf("%d",&accountT);
    pass:
    printf("请输入当前密码：（密码由数字、字母、下划线组成且不超过16位）\n");
    memset(passT,0,sizeof(passT));
    scanf("%s",passT);
    if(strlen(passT)==0)
    {
        printf("密码不能为空！\n");
        goto pass;
    }
    if(strlen(passT)>16)
    {
        printf("密码过长！\n");
        goto pass;
    }
    strcpy(msg.pass,passT);
    msg.account=accountT;
    send(sock,&msg,sizeof (MSG),0);
    if(recv(sock,&msg,sizeof(MSG),0) <= 0) //每次都判断
        {
            printf("服务器断开链接\n");
            exit(-1);
        }
    if(msg.flag==1)
    {
        pass2:
        printf("请输入新密码：（密码由数字、字母、下划线组成且不超过16位）\n");
        memset(passT,0,sizeof(passT));
        scanf("%s",passT);
        if(strlen(passT)==0)
        {
            printf("密码不能为空！\n");
            goto pass;
        }
        if(strlen(passT)>16)
        {
            printf("密码过长！\n");
            goto pass2;
        }
        strcpy(msg.pass,passT);
        send(sock,&msg,sizeof (MSG),0);
        if(recv(sock,&msg,sizeof(MSG),0) <= 0) //每次都判断
            {
                printf("服务器断开链接\n");
                exit(-1);
            }
        if(msg.flag==1)
        {
            printf("修改成功！\n");
            return;
        }
    }
    else
    {
        printf("原密码错误！是否继续：（y/n）\n");
        char ch;
        scanf("%c%c",&ch,&ch);
        if(ch=='y'||ch=='Y')
        {
            goto pass;
        }
        else
        {
            return;
        }

    }
}

void changeUsername(int sock)          //修改昵称
{
    MSG msg;
    msg.work=8;
    int accountT;
    char passT[30];                  //用户密码
    char nameT[30];
    printf("\x1b[H\x1b[2J");        //清屏
    printf("*******************修改昵称页面*******************\n");
    printf("请输入需要修改帐号：\n");
    scanf("%d",&accountT);
    pass:
    printf("请输入当前密码：（密码由数字、字母、下划线组成且不超过16位）\n");
    memset(passT,0,sizeof(passT));
    scanf("%s",passT);
    if(strlen(passT)==0)
    {
        printf("密码不能为空！\n");
        goto pass;
    }
    if(strlen(passT)>16)
    {
        printf("密码过长！\n");
        goto pass;
    }
    strcpy(msg.pass,passT);
    msg.account=accountT;
    send(sock,&msg,sizeof (MSG),0);
    if(recv(sock,&msg,sizeof(MSG),0) <= 0) //每次都判断
        {
            printf("服务器断开链接\n");
            exit(-1);
        }
    if(msg.flag==1)
    {
        name:
        printf("请输入新昵称：（不超过14个字符）\n");
        memset(nameT,0,sizeof(nameT));
        scanf("%s",nameT);
        if(strlen(nameT)==0)
        {
            printf("昵称不能为空！\n");
            goto name;
        }
        if(strlen(nameT)>14)
        {
            printf("昵称过长！\n");
            goto name;
        }
        strcpy(msg.name,nameT);
        send(sock,&msg,sizeof (MSG),0);
        if(recv(sock,&msg,sizeof(MSG),0) <= 0) //每次都判断
            {
                printf("服务器断开链接\n");
                exit(-1);
            }
        if(msg.flag==1)
        {
            printf("修改成功！\n");
            strcpy(myName,nameT);
            return;
        }
    }
    else
    {
        printf("原密码错误！是否继续：（y/n）\n");
        char ch;
        scanf("%c%c",&ch,&ch);
        if(ch=='y'||ch=='Y')
        {
            goto pass;
        }
        else
        {
            return;
        }

    }
}

void recoverPassword(int sock)         //找回密码
{
     MSG msg;
     msg.work=11;
     int accountT;
     char answerT[256];               //密保答案
     printf("\x1b[H\x1b[2J");        //清屏
     printf("*******************找回页面*******************\n");
     printf("请输入需要找回的帐号：\n");
     scanf("%d",&accountT);
     msg.account=accountT;
     send(sock,&msg,sizeof (MSG),0);
     if(recv(sock,&msg,sizeof(MSG),0) <= 0) //每次都判断
     {
         printf("服务器断开链接\n");
         exit(-1);
     }
     printf("\n%s?\n\n",msg.question);
    answer:
    printf("请输密保答案：（不超过50个字）\n");
    memset(answerT,0,sizeof(answerT));
    scanf("%s",answerT);
    if(strlen(answerT)==0)
    {
        printf("答案不能为空！\n");
        goto answer;
    }
    if(strlen(answerT)>120)
    {
        printf("字数过长！\n");
        goto answer;
    }
    strcpy(msg.answer,answerT);
    send(sock,&msg,sizeof (MSG),0);
    if(recv(sock,&msg,sizeof(MSG),0) <= 0) //每次都判断
    {
        printf("服务器断开链接\n");
        exit(-1);
    }
    if(msg.flag==1)
    {
        printf("您的密码为：%s\n",msg.pass);
    }
    else
    {
        printf("答案错误！\n");
    }
}

void* recvThread(void *cSock)          //接收线程
{
//    void *c=cSock;
//    int sock=*(int*)c;
    MSG msg;
    printf("开始接收消息！\n");
    while(1)
    {
        if(recv(clientSock,&msg,sizeof (MSG),0)<=0)
        {
            printf("服务器断开！\n");
            exit(-1);
        }
        switch(msg.work)
        {
        case 3:
            printf("%s（%d）(群聊)：%s\n",msg.name,msg.account,msg.mess);
            break;
        case 5:
            printf("%s（%d）私聊你：%s\n",msg.name,msg.account,msg.mess);
            break;
        case 6:
           printf("%s（%d）在线\n",msg.name,msg.account);
           break;
        case 12:
           printf("%s（%d）在群中\n",msg.name,msg.account);
           break;

        }
        if(msg.work==66)
            break;
    }


    return NULL;
}

void exitAccount(int sock)              //退出帐号
{
    MSG msg;
    msg.work=9;
    msg.account=myAccount;
    strcpy(msg.name,myName);
    send(sock,&msg,sizeof (MSG),0);
}

void lookOnline(int sock)               //查看在线成员
{
    MSG msg;
    msg.work=6;
    msg.account=myAccount;
    strcpy(msg.name,myName);
    send(sock,&msg,sizeof (MSG),0);
}

void lookGL(int sock)                   //查看在群聊中的人员
{
    MSG msg;
    msg.work=12;
    msg.account=myAccount;
    strcpy(msg.name,myName);
    send(sock,&msg,sizeof (MSG),0);
}

void chatManual()                       //群聊手册
{
    printf("*******************Help*********************\n");
    printf("\t群聊操作指南\n");
    printf("q3：查看群聊在线人员\n");
    printf("q4：退出群聊，返回主程序\n");
    printf("********************************************\n");
}

void chatOneMau()                       //私聊手册
{
    printf("*******************Help*********************\n");
    printf("\t私聊操作指南\n");
    printf("\t1：查看在线人员\n");
    printf("\t2：开始私聊\n");
    printf("\t3：退出私聊，返回主程序\n");
    printf("********************************************\n");
}

void chatWhile(int sock,MSG *msg)       //私聊循环
{
    char buf[1024];
    char temp[10];
    printf("输入私聊目标：（帐号）\n");
    scanf("%s",temp);
    int detAccount=atoi(temp);
    msg->detAccount=detAccount;
    while(1)
    {
        printf("输入q4退出\n");
        scanf("%s",buf);
        strcpy(msg->mess,buf);
        if (strcmp(msg->mess,"q4") == 0)
        {
            printf("已退出\n");
            break;
        }
        else
        {
            send(sock,msg,sizeof (MSG),0);
            sleep(1);
        }
    }
}

void chatAll(int sock)                  //群聊
{
    MSG msg;
    char buf[1024];
    msg.work=10;
    msg.flag=1;
    msg.account=myAccount;
    strcpy(msg.name,myName);
    send(sock,&msg,sizeof (MSG),0);
    printf("\x1b[H\x1b[2J");        //清屏
    printf("*******群聊页面*******\n");
    chatManual();
    while(1)
    {
        msg.flag=0;
        scanf("%s",buf);
        strcpy(msg.mess,buf);
        if (strcmp(msg.mess,"q4") == 0)					//输入 bye 表示退出群聊
        {
            memset(buf,0,sizeof(buf));
            sprintf(msg.mess,"我退出了群聊\n");
            send(sock,&msg,sizeof(MSG),0);		//将退出的消息发给公屏
            memset(msg.mess,0,strlen(msg.mess));
            msg.work=3;
            strcpy(msg.mess,"q4");
            send(sock,&msg,sizeof(MSG),0);
            break;
        }
        else if(strcmp(msg.mess,"q3") == 0)
        {
            lookGL(sock);
            sleep(1);
        }
        else
        {
            msg.work=3;
            send(sock,&msg,sizeof (MSG),0);
            sleep(1);
        }
    }

}

void chatOne(int sock)                  //私聊
{
    MSG msg;
    msg.work=5;
    msg.account=myAccount;
    strcpy(msg.name,myName);
    printf("\x1b[H\x1b[2J");        //清屏
    printf("*******私聊页面*******\n");
    while(1)
    {
        chatOneMau();
        char b[5];
        scanf("%s",b);
        int i=atoi(b);
        switch (i)
        {
        case 1:
            lookOnline(sock);
            sleep(1);
            break;
        case 2:
            chatWhile(sock,&msg);
            break;
        case 3:
            i=4;
            break;
        }
        if(i==4)
        {
            break;
        }
    }
}

void chatAction()                       //action二级菜单
{
    int work;
    char b[5];
    while(1)
    {
        sleep(1);
        printf("\t****************\n");
        printf("\t3.进入群聊\n");
        printf("\t4.私聊\n");
        printf("\t5.查看在线人员\n");
        printf("\t6.退出到主程序\n");
        printf("\t****************\n");
        scanf("%s",b);
        work=atoi(b);
        switch (work)
        {
            case 3:
                chatAll(clientSock);
                break;
            case 4:
                chatOne(clientSock);
                break;
            case 6:
                exitAccount(clientSock);
                break;                
            case 5:
                lookOnline(clientSock);
                break;
        }
        if(work==6)
        {
            break;
        }

    }

}


#endif // CLIENT_H
