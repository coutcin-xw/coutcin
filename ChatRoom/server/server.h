#ifndef SERVER_H
#define SERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<stdbool.h>
#define PORT 7777                                   //定义端口宏

typedef struct MsgData
{
    int flag;                       //标识
    int work;                       //操作选项
    int  account;                   //用户登入账号
    int detAccount;                 //私聊时目标帐号
    char mess[1024];                //消息
    char name[30];                  //用户昵称
    char pass[30];                  //用户密码
    char question[256];             //密保问题
    char answer[256];               //密保答案
    char time[30];                  //时间
} MSG;

typedef struct User
{
    int  account;                   //用户登入账号
    char name[30];                  //用户昵称
    char pass[30];                  //用户密码
    char question[256];             //密保问题
    char answer[256];               //密保答案
    struct User* pNext;             //下个节点指针

} USER;

typedef struct GroupList
{
    int  account;                   //用户登入账号
    char name[30];                  //用户昵称
    int copySock;                   //拷贝sock
    bool isInChat;                  //在群聊否  1在0不在,是on chat??
    bool state;                     //主动状态，融合私聊 *	0；正常状态1；请勿打扰（禁止私聊）
    struct GroupList* pNext;        //下个节点
} GL;

int intSocket()                     //初始化服务器套接字
{
    int fdsock=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    memset(&addr,0,sizeof (addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);         //addr.sin_addr.s_addr = inet_addr(IP);

    int binds=bind(fdsock,(struct sockaddr*)&addr,sizeof(addr));
    if(binds==-1)
    {
        perror("绑定失败!");
        exit(-1);
    }
    if(listen(fdsock,100)==-1)
    {
        perror("监听失败");
        exit(-1);
    }
    printf("服务器已开启，正在监听中......\n");
    return fdsock;
}

int myAccept(int serverfd)          //接受连接函数
{
    struct sockaddr_in client_addr;// 用来保存客户端的ip和端口信息
        int len = sizeof(client_addr);
        int clientfd = accept(serverfd,(struct sockaddr *)&client_addr,  (socklen_t*)&len);
        if (clientfd == -1)
        {
            perror ("accept");
        }
        printf ("成功接收一个客户端: %s\n", inet_ntoa(client_addr.sin_addr));
        return clientfd;
}

GL* intiGL()                        //初始化群聊成员列表
{

    GL *p = (GL *)malloc(sizeof(GL));
    if(p == NULL)
    {
        printf("ERRER\n");
        exit(-1);
    }
    p->pNext = NULL;
    return p;
}

void addGL(int  account,char name[30],int copySock,bool isInChat,bool state,GL *pH)                     //添加群聊成员
{
    GL* pEnd=pH;
    while(pEnd->pNext!=NULL)
    {
       pEnd=pEnd->pNext;
    }
    GL *p=(GL *)malloc(sizeof(GL));
    if(p == NULL)
    {
       printf("ERRER\n");
       exit(-1);
    }
    memset(p,0,sizeof (GL));
    strcpy(p->name,name);
    p->account=account;
    p->copySock=copySock;
    p->state=state;
    p->isInChat=isInChat;

    pEnd->pNext=p;
    p->pNext=NULL;

}

void addUSER(int  account,char name[30],char pass[30],char question[256],char answer[256],USER* pH)     //添加用户
{
    USER* pEnd=pH;
    while(pEnd->pNext!=NULL)
    {
       pEnd=pEnd->pNext;
    }
    USER *p = (USER *)malloc(sizeof(USER));
    if(p == NULL)
    {
       printf("ERRER\n");
       exit(-1);
    }
    memset(p,0,sizeof (USER));
    strcpy(p->name,name);
    strcpy(p->pass,pass);
    strcpy(p->answer,answer);
    strcpy(p->question,question);
    p->account=account;
    pEnd->pNext=p;
    p->pNext=NULL;
}

USER* intiUSER()                                            //初始化用户数据列表
{
    USER *p = (USER *)malloc(sizeof(USER));
    if(p == NULL)
    {
        printf("ERRER\n");
        exit(-1);
    }
    p->pNext = NULL;

    FILE *fp=NULL;
    fp=fopen("user.txt","a+");
    char account[6];
    char name[30];                  //用户昵称
    char pass[30];                  //用户密码
    char question[256];             //密保问题
    char answer[256];               //密保答案
    char data[583];
    int t=0;
    while(NULL!=fgets(data,583,fp))
    {
        int j=0;
        t=0;
        for(int i=0;data[i]!='\n';i++)
        {
            if(t==0)
            {
                if(data[i]=='*')
                {
                    t++;
                    account[i]='\0';
                    j=0;

                }
                else
                {
                    account[j]=data[i];
                    j++;
                }
            }else if(t==1)
            {
                if(data[i]=='*')
                {
                    t++;
                    name[j]='\0';
                    j=0;
                }
                else
                {
                    name[j]=data[i];
                    j++;
                }
            }else if(t==2)
            {
                if(data[i]=='*')
                {
                    t++;
                    pass[j]='\0';
                    j=0;
                }
                else
                {
                    pass[j]=data[i];
                    j++;
                }
            }else if(t==3)
            {
                if(data[i]=='*')
                {
                    t++;
                    question[j]='\0';
                    j=0;
                }
                else
                {
                    question[j]=data[i];
                    j++;
                }
            }else if(t==4)
            {
                if(data[i]=='*')
                {
                    t++;
                    answer[j]='\0';
                    j=0;
                }
                else
                {
                    answer[j]=data[i];
                    j++;
                }
            }
        }
        int a=atoi(account);
        addUSER(a,name,pass,question,answer,p);
    }
    fclose(fp);
    return p;
}

GL* findGL(GL* pH,int account)                              //查找群聊成员
{
    GL* p=pH;
    while(p!=NULL)
    {
        if(p->account==account)
        {
            return p;
        }
        p=p->pNext;
    }
    return NULL;
}

USER* findUSER(USER* pH,int account)                        //查找用户
{
    USER* p=pH;
    while(p!=NULL)
    {
        if(p->account==account)
        {
            return p;
        }
        p=p->pNext;
    }
    return NULL;
}

void deleteGL(GL* pH,int account)                           //删除群聊成员
{
    GL* p=pH->pNext;
    GL* pN=pH;

    while(p!=NULL)
    {
        if(p->account==account)
        {
            pN->pNext=p->pNext;
            free(p);
            return ;
        }
        p=p->pNext;
        pN=pN->pNext;
//        pN=pN->pNext;
//        p=pN->pNext;
    }
}

void deleteUSER(USER* pH,int account)                       //删除用户
{
    USER* p=pH->pNext;
    USER* pN=pH;

    while(p!=NULL)
    {
        if(p->account==account)
        {
            pN->pNext=p->pNext;
            free(p);
            return ;
        }
        p=p->pNext;
        pN=pN->pNext;
//        pN=pN->pNext;
//        p=pN->pNext;
    }
}

void modifyGL(int temp,int  account,char name[30],int copySock,bool isInChat,bool state,GL *pH)        //修改群聊成员
{
    GL*p=findGL(pH,account);
    if(p==NULL)
    {
//        printf("find error!\n");
        return ;
    }
    switch(temp)
    {
    case 1:
        strcpy(p->name,name);
        break;
    case 2:
        p->copySock=copySock;
        break;
    case 3:
        p->isInChat=isInChat;
        break;
    case 4:
        p->state=state;
        break;
    default:
        printf("temp error!\n");
    }

}

void modifyUSER(int temp,int  account,char name[30],char pass[30],char question[256],char answer[256],USER* pH)//修改用户信息
{
    USER *p=findUSER(pH,account);
    if(p==NULL)
    {
        printf("find error!\n");
        return ;
    }
    switch(temp)
    {
    case 1:
        strcpy(p->name,name);
        break;
    case 2:
        strcpy(p->pass,pass);
        break;
    case 3:
        strcpy(p->question,question);
        break;
    case 4:
        strcpy(p->answer,answer);
        break;
    default:
        printf("temp error!\n");
    }

}

void registerAccount(int cSock,MSG *msg,USER* pH)           //注册帐号
{
    msg->work=1;
    USER *p=findUSER(pH,msg->account);
    if(p!=NULL)
    {
        msg->flag=3;        //重名了
    }
    else
    {
        addUSER(msg->account,msg->name,msg->pass,msg->question,msg->answer,pH);
        msg->flag=1;        //成功
    }
    send(cSock,msg,sizeof(MSG),0);
}

int checkPass(MSG *msg,USER* pH)                            //检验密码
{
    USER *p=findUSER(pH,msg->account);
    if(p!=NULL)
    {
        if(strcmp(msg->pass,p->pass)==0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return -1;
}

int checkAnswer(MSG *msg,USER* pH)                          //检验密保答案
{
    USER *p=findUSER(pH,msg->account);
    if(p!=NULL)
    {
        if(strcmp(msg->answer,p->answer)==0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return -1;
}

void logInAccount(int cSock,MSG *msg,USER* pH,GL *pGH)      //登录
{
    if(findUSER(pH,msg->account)==NULL)
    {
        msg->flag=2;    //未注册
    }
    else if(findGL(pGH,msg->account)!=NULL)
    {
        msg->flag=3;    //已上线
    }
    else if(findGL(pGH,msg->account)==NULL)
    {
        int temp=checkPass(msg,pH);
        if(temp==1)
        {
            msg->flag=1;    //登录成功
            USER *p=findUSER(pH,msg->account);
            if(p!=NULL)
            addGL(msg->account,p->name,cSock,false,true,pGH);
            strcpy(msg->name,p->name);
            printf("%s 登录成功！\n",p->name);
        }else
        {
            msg->flag=4;    //密码错误
        }
    }
    send(cSock,msg,sizeof(MSG),0);

}

void changePassword(int sock,MSG *msg,USER* pH)             //修改密码
{
    if(checkPass(msg,pH)==1)
    {
        msg->flag=1;            //密码正确
        send(sock,msg,sizeof(MSG),0);
        if(recv(sock,msg,sizeof(MSG),0)<=0)
        {
            USER*p=findUSER(pH,msg->account);
            printf("%s (%d)已下线！\n",p->name,msg->account);
            return ;
        }
        else
        {
            modifyUSER(2,msg->account,NULL,msg->pass,NULL,NULL,pH);
            msg->flag=1;    //修改成功
            send(sock,msg,sizeof(MSG),0);
        }
    }
    else
    {
        msg->flag=2;            //密码错误
        send(sock,msg,sizeof(MSG),0);
    }
}

void changeUsername(int sock,MSG *msg,USER* pH,GL* pGH)     //修改昵称
{
    if(checkPass(msg,pH)==1)
    {
        msg->flag=1;            //密码正确
        send(sock,msg,sizeof(MSG),0);
        if(recv(sock,msg,sizeof(MSG),0)<=0)
        {
            USER*p=findUSER(pH,msg->account);
            printf("%s (%d)已下线！\n",p->name,msg->account);
            return ;
        }
        else
        {
            modifyUSER(1,msg->account,msg->name,NULL,NULL,NULL,pH);
            modifyGL(1,msg->account,msg->name,0,0,0,pGH);
            msg->flag=1;    //修改成功
            send(sock,msg,sizeof(MSG),0);
        }
    }
    else
    {
        msg->flag=2;            //密码错误
        send(sock,msg,sizeof(MSG),0);
    }
}

void recoverPassword(int sock,MSG *msg,USER* pH)            //找回密码
{
    USER* p=findUSER(pH,msg->account);
    strcpy(msg->question,p->question);
    send(sock,msg,sizeof (MSG),0);
    if(recv(sock,msg,sizeof(MSG),0)<=0)
    {
        printf("%s (%d)已下线！\n",p->name,msg->account);
        return ;
    }
    if(checkAnswer(msg,pH)==1)
    {
        msg->flag=1;
        strcpy(msg->pass,p->pass);
    }
    else
    {
        msg->flag=2;

    }
     send(sock,msg,sizeof (MSG),0);
}

void saveUserData(USER* pH)                                 //保存用户数据
{
    FILE *fp=NULL;
    USER* p=pH->pNext;
    fp=fopen("user.txt","w+");
    while(p!=NULL)
    {
        char account[6];
        memset(account,0,sizeof (account));
        sprintf(account, "%d", p->account);
        fprintf(fp,account);
        fprintf(fp,"*");
        fprintf(fp,p->name);
        fprintf(fp,"*");
        fprintf(fp,p->pass);
        fprintf(fp,"*");
        fprintf(fp,p->question);
        fprintf(fp,"*");
        fprintf(fp,p->answer);
        fprintf(fp,"\n");
        p=p->pNext;
    }
    fclose(fp);

}

void exitChatAll(GL *pGH,int account)                       //退出群聊
{
    GL* p=findGL(pGH,account);
    if(p!=NULL)
    {
        p->isInChat=0;
        printf("%s（%d）退出了群聊!\n",p->name,p->account);
    }
    else
    {
        printf("exitChatAll() error!\n");
    }
}

int isCanPrivateChat(GL* pGH,int account)                   //是否可以私聊
{
    GL *p=findGL(pGH,account);
    if(p->state==1)
    {
        return 1;
    }
    return 0;
}

void chatAll(int sock,GL* pGH,MSG *msg)                     //群聊
{
    GL *p=pGH->pNext;
    if(strcmp(msg->mess,"q4") == 0)
        {
            exitChatAll(pGH,msg->account);
            return;
        }
        else
        {
            while(p != NULL)
            {
                if(p->isInChat==1)
                {
                    send(p->copySock,msg,sizeof (MSG),0);
                }
                p = p->pNext;
            }
        }
}

void lookGL(int sock,GL* pGH,MSG* msg)                      //查看群聊成员 或者在线成员
{
    GL *p=pGH->pNext;

    while(p!=NULL)
    {
        if(p->isInChat==1)
        {
            strcpy(msg->name,p->name);
            msg->account=p->account;
            send(sock,msg,sizeof (MSG),0);
        }
        p=p->pNext;
    }
}

void lookOnline(int sock,GL* pGH,MSG* msg)                  //查看在线成员
{
    GL *p=pGH->pNext;
    while(p!=NULL)
    {

            strcpy(msg->name,p->name);
            msg->account=p->account;
            send(sock,msg,sizeof (MSG),0);
        p=p->pNext;
    }
}

void enterGroupChat(int sock,GL* pGH,MSG *msg)              //进入群聊
{
    if(msg->flag==1)
    {
        GL* p=findGL(pGH,msg->account);
        if(p!=NULL)
        {
            p->isInChat=1;
            printf("%s（%d）进入了群聊\n",msg->name,msg->account);
        }
        else
        {
            printf("exitChatAll() error!\n");
        }
    }
}

void exitLink(int sock,GL* pGH,MSG* msg)                    //下线
{
    GL* p=findGL(pGH,msg->account);
    if(p!=NULL)
    {
        printf("%s 已下线！\n",p->name);
        deleteGL(pGH,msg->account);

    }else
    {
        printf("不在线！\n");
    }
}

void chatOne(int sock,GL* pGH,MSG* msg)                     //私聊
{
    GL* p=findGL(pGH,msg->detAccount);
    if(p!=NULL)
    {
        if(p->state==1)
        {
            send(p->copySock,msg,sizeof (MSG),0);
        }
        else
        {
            msg->flag=2;
            send(sock,msg,sizeof (MSG),0);
        }
    }
}

#endif // SERVER_H
