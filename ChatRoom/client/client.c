#include "client.h"

void action();                          //客户端执行操作
void homePage();                        //首页
void registeredAccount(int sock);       //注册帐号
bool logIn(int sock);                   //登录
void changePassword(int sock);          //修改密码
void changeUsername(int sock);          //修改昵称
void recoverPassword(int sock);         //找回密码
void* recvThread(void *cSock);          //线程回调函数
void chatAction();                      //action二级菜单
void chatManual();                      //群聊手册
void exitAccount(int sock);             //下线
void lookOnline(int sock);              //查看在线成员
void chatAll(int sock);                 //群聊
void chatOne(int sock);                 //私聊
void lookGL(int sock);                  //查看在群聊中的人员
void chatOneMau();                      //私聊手册
void chatWhile(int sock,MSG *msg);      //私聊循环

int main()
{
    struct sockaddr_in addr;
    clientSock=socket(PF_INET,SOCK_STREAM,0);
    if(clientSock==-1)
    {
        perror ("socket");      //C 库函数 void perror(const char *str) 把一个描述性错误消息输出到标准错误 stderr
        return -1;
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=inet_addr(IP);

    int cnt=connect(clientSock,(struct sockaddr*)&addr,sizeof (addr));
    if(cnt==-1)
    {
        perror("connect error");
        return -1;
    }
    printf("客户端成功连接服务器，服务器ip：%s\n",IP);
    action();
    close(clientSock);
    return 0;
}

void action()
{
    homePage();
    int actions;
    char ch[5]; //使用字符串读入 防止输入错误
    while(1)
    {

        printf("输入操作：（输入0:查看菜单）\n");
//        gets(ch);                           //读取 函数'get '的隐式声明在C99中无效
        scanf("%s",ch);
        actions=atoi(ch);
        switch(actions)
        {
            case 0:
                homePage();
                break;
            case 1:
                registeredAccount(clientSock);        //注册帐号
                break;
            case 2:
                if(logIn(clientSock))                 //登录成功创建接收线程
                {
                     pthread_t t_id;
                     pthread_create(&t_id,NULL,recvThread,(void*)clientSock);
                     pthread_detach(t_id);
                     chatAction();
                }
                break;
            case 3:
                changePassword(clientSock);          //修改密码
                break;
            case 4:
                changeUsername(clientSock);          //修改昵称
                break;
            case 5:
                recoverPassword(clientSock);         //找回密码
                break;
            case 6:
                close(clientSock);
                exit(0);                             //退出
                break;
        }
        memset(ch,0,strlen(ch));
    }
}
