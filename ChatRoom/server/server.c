#include "server.h"

int intSocket();                        //初始化服务器套接字
void* action(void* sockfd);             //线程回调函数
int myAccept(int serverfd);             //接受连接函数
GL* intiGL();                           //初始化群聊成员列表
USER* intiUSER();                       //初始化用户数据列表
void addUSER(int  account,char name[30],char pass[30],char question[256],char answer[256],USER* pH);     //添加用户
void addGL(int  account,char name[30],int copySock,bool isInChat,bool state,GL *pH);                     //添加群聊成员
GL* findGL(GL* pH,int account);                                                  //查找群聊成员
USER* findUSER(USER* pH,int account);                                            //查找用户
void deleteGL(GL* pH,int account);                                               //删除群聊成员
void deleteUSER(USER* pH,int account);                                           //删除用户
void modifyGL(int temp,int  account,char name[30],int copySock,bool isInChat,bool state,GL *pH);                //修改群聊成员
void modifyUSER(int temp,int  account,char name[30],char pass[30],char question[256],char answer[256],USER* pH);//修改用户信息

void registerAccount(int cSock,MSG *msg,USER* pH);                               //注册帐号
void logInAccount(int cSock,MSG *msg,USER* pH,GL *pGH);                          //登录
int checkPass(MSG *msg,USER* pH);                                                //检验密码
int checkAnswer(MSG *msg,USER* pH);                                              //检验密保答案
void changePassword(int sock,MSG *msg,USER* pH);                                 //修改密码
void changeUsername(int sock,MSG *msg,USER* pH,GL* pGH);                         //修改昵称
void recoverPassword(int sock,MSG *msg,USER* pH);                                //找回密码

void saveUserData(USER* pH);
void enterGroupChat(int sock,GL* pGH,MSG *msg);                                  //进入群聊
void chatAll(int sock,GL* pGH,MSG *msg);                                         //群聊
void lookGL(int sock,GL* pGH,MSG* msg);                                          //查看群聊成员
int isCanPrivateChat(GL* pGH,int account);                                       //是否可以私聊
void exitChatAll(GL *pGH,int account);                                           //退出群聊
void lookOnline(int sock,GL* pGH,MSG* msg);                                      //查看在线成员
void exitLink(int sock,GL* pGH,MSG* msg);                                        // 下线
void chatOne(int sock,GL* pGH,MSG* msg);                                         //私聊

int serverSock;                     //服务器socket
int clientSock;                     //客户端socket
USER *pH=NULL;                      //用户链表；
GL *pGH=NULL;


int main()
{
    serverSock=intSocket();                                         //初始化服务器套接字
    pH=intiUSER();                                                  //初始化用户链表
    pGH=intiGL();                                                   //初始化在线用户列表
    while(1)
    {
        int clientfd;
        clientfd=myAccept(serverSock);                              //接受连接

        pthread_t t_id;
        pthread_create(&t_id,NULL,action,(void*)&clientfd);         //创建该客户端线程
        pthread_detach(t_id);
    }
    close(serverSock);
    return 0;
}

void* action(void* sockfd)          //线程回调函数
{
    int cSock=*(int*)sockfd;
    struct MsgData msg;

    while(1)
    {
        if(recv(cSock,&msg,sizeof(msg),0)<=0)
        {
            USER*p=findUSER(pH,msg.account);
            printf("%s (%d)已下线！\n",p->name,msg.account);
            deleteGL(pGH,msg.account);
            break;
        }
        switch(msg.work)
        {
        case 1:
            registerAccount(cSock,&msg,pH);             //注册
            saveUserData(pH);                           //保存用户数据到文件
            break;
        case 2:
            logInAccount(cSock,&msg,pH,pGH);            //登录
            break;
        case 3:
            chatAll(cSock,pGH,&msg);                    //群聊
            break;
//        case 4:
//            //检验对象的状态，在线或者是否接受私聊
//            break;
        case 5:
            chatOne(cSock,pGH,&msg);                        //私聊
            break;
        case 6:
            lookOnline(cSock,pGH,&msg);                    //查看在线用户
            break;
        case 7:
            changePassword(cSock,&msg,pH);              //修改密码
            saveUserData(pH);                           //保存用户数据到文件
            break;
        case 8:
            changeUsername(cSock,&msg,pH,pGH);          //修改昵称
            saveUserData(pH);                           //保存用户数据到文件
            break;
        case 9:
            exitLink(cSock,pGH,&msg);                   //下线
            msg.work=66;
            send(cSock,&msg,sizeof (MSG),0);
            break;
        case 10:
            enterGroupChat(cSock,pGH,&msg);             //进入群聊
            break;
        case 11:
            recoverPassword(cSock,&msg,pH);
            break;
        case 12:
            lookGL(cSock,pGH,&msg);
            break;
        }
    }
    close(cSock);
    return NULL;
}
