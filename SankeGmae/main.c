/*
The project name:SnakeGame
Author: CoutCin
Time:2020.03.06 - 2020.03.07
*/

#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<mmsystem.h>
#include<conio.h>
#include<time.h>
#include<stdbool.h>
#pragma comment(lib,"winmm.lib")

#define SnakMax 20

//定义蛇节点结构
typedef struct Snake 
{
	int x;
	int y;
	int Dec;
	_Bool s;
} SNAKE;
//定义方向联合体
enum Diraction
{
	T_E = 1,
	T_W = 2,
	T_S = 3,
	T_N = 4
};

void Home_Page();//显示首页

void Play_Music();//播放音乐

void Detection_Of_Space();//按空格键

void Stop_Music();//停止播放

void PrintInterface();//打印背景边框
 
void SetRandomNumber();//设置随机数

void SetSnake();//画蛇

void SnakeMobile();//蛇移动

void ResetBackg();//重设背景

void ChangeDir();//设置方向

bool DeathToJudge();//死亡判断

void ProduceFood();//产生食物

bool JudgeFoot();//判断食物

void LetSnake();//变长

//定义背景数组
short g_arrBackground[20][23] = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
								{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };

//定义蛇的身体数组
SNAKE S_gSnake[SnakMax];

int Fx = -1;
int Fy = -1;

int main(void)
{

	Home_Page();//显示首页

	Play_Music();//播放音乐
	
	Detection_Of_Space();//按空格键

	system("cls");//清空
	
	SetRandomNumber();//随机出现蛇
	ProduceFood();
	//游戏界面
	while (1)//游戏主循环
	{
		ChangeDir();//检测输入

		if (DeathToJudge())//判断死亡
		{
			system("cls");//清空	
			printf("小蛇死亡 游戏失败！\n");
			break;
		}
		LetSnake();//吃食物变长

		SnakeMobile();//移动蛇 
	
		PrintInterface(); //打印界面

		Sleep(500);
	}
	Stop_Music();//停止播放

	system("Pause");
	return 0;
}


//首页显示
void Home_Page()
{
	printf("   《欢迎来到贪吃蛇的世界》\n");
	printf("     《按空格键开始游戏》\n");
	printf("《按 w a s d 控制贪吃蛇的轨迹》\n");
}
//播放音乐
void Play_Music()
{
	PlaySound(L"D:\\vs2019\\SnakeGame\\1.WAV", NULL, SND_FILENAME | SND_ASYNC);//参数一填.WAV格式文件的绝对路径 只可以.MAV格式的音频文件
}
//停止播放音乐
void Stop_Music()
{
	PlaySound(NULL, 0, 0);
}
//检测是否输入空格
void Detection_Of_Space()
{
	char chIput;
	while (1)
	{
		chIput = _getch();
		if (' ' == chIput)
		{
			break;
		}
	}
}

//打印边框
void PrintInterface()
{
		system("cls");
		int i = 0;
		int j = 0;
		for (i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				if (1 == g_arrBackground[i][j])
					printf("█");
				else if(2 == g_arrBackground[i][j])
					printf("◆");

				else
					printf("  ");
			}
			printf("\n");
		}
}

//设置随机数
void SetRandomNumber()
{
	int nX = -1;
	int nY = -1;
	//产生随机数
	srand((unsigned int )time(NULL));
	 
	nX = rand() % 16 + 1;
	nY = rand() % 18 + 1;
	for (int i = 0; i < 3; i++)
	{
		S_gSnake[i].x = nX+i;
		S_gSnake[i].y = nY;
		S_gSnake[i].Dec = T_W;
		S_gSnake[i].s = true;
	}
	SetSnake();
}
void SetSnake()
{
	for (int i = 0; i < SnakMax; i++)
	{
		if(S_gSnake[i].s&& g_arrBackground[S_gSnake[i].y][S_gSnake[i].x]!=1)
		g_arrBackground[S_gSnake[i].y][S_gSnake[i].x]=1;

	}
}

//蛇的移动
void SnakeMobile()
{
	ResetBackg();
	int i = SnakMax - 1;
	int flag = 0;
	for (i; i >= 1; i--)
	{
		if (S_gSnake[i].s)
		{
			S_gSnake[i].x = S_gSnake[i - 1].x;
			S_gSnake[i].y = S_gSnake[i - 1].y;
			S_gSnake[i].Dec = S_gSnake[i - 1].Dec;
		}
	}
	switch (S_gSnake[0].Dec)
	{
	case T_E:
		S_gSnake[0].x += 1;
		break;
	case T_W:
		S_gSnake[0].x -= 1;
		break;
	case T_S:
		S_gSnake[0].y += 1;
		break;
	case T_N:
		S_gSnake[0].y -= 1;
		break;
	}
	SetSnake();
}

//重设背景
void ResetBackg()
{
	for (int i = 0; i < SnakMax; i++)
	{
		if (S_gSnake[i].s)
			g_arrBackground[S_gSnake[i].y][S_gSnake[i].x] = 0;
	}
}

//设置方向
void ChangeDir()
{
	//高字节  ：
	//低字节：
	if (GetAsyncKeyState('W'))
	{
		S_gSnake[0].Dec = T_N;
	}
	if (GetAsyncKeyState('A'))
	{
		S_gSnake[0].Dec = T_W;
	}
	if (GetAsyncKeyState('S'))
	{
		S_gSnake[0].Dec = T_S;
	}
	if (GetAsyncKeyState('D'))
	{
		S_gSnake[0].Dec = T_E;
	}
}
//死亡判断
bool DeathToJudge()
{
	
	switch (S_gSnake[0].Dec)
	{
	case T_E:
		if (g_arrBackground[S_gSnake[0].y][S_gSnake[0].x + 1] == 1)
			return true;
		break;
	case T_W:
		if (g_arrBackground[S_gSnake[0].y][S_gSnake[0].x-1] == 1)
			return true;
		break;
	case T_S:
		if (g_arrBackground[S_gSnake[0].y+1][S_gSnake[0].x] == 1)
			return true;
		break;
	case T_N:
		if (g_arrBackground[S_gSnake[0].y-1][S_gSnake[0].x] == 1)
			return true;
		break;
	
	}
		return false;
}

//产生食物
void ProduceFood()
{
	int nX = -1;
	int nY = -1;
	//产生随机数
	srand((unsigned int)time(NULL));
	fn:
	nX = rand() % 18 + 1;
	nY = rand() % 18 + 1;

	if (g_arrBackground[nY][nX] == 0)
	{
		g_arrBackground[nY][nX] = 2;
		Fx = nX;
		Fy = nY;
	}
	else
	{
		goto fn;
	}

}

//判断食物
bool JudgeFoot()
{
	if (S_gSnake[0].x == Fx && S_gSnake[0].y == Fy)
		return true;

	return false;
}

//变长
void LetSnake()
{
	int t = 0;
	if (JudgeFoot())
	{
		for (int i = 0; i < SnakMax; i++)
		{
			if (t == 0 && !S_gSnake[i].s&&i>2)
			{
				switch (S_gSnake[0].Dec)
				{
				case T_E:
					S_gSnake[i].x = S_gSnake[i - 1].x - 1;
					S_gSnake[i].y = S_gSnake[i - 1].y;
					S_gSnake[i].Dec = S_gSnake[i - 1].Dec;
					S_gSnake[i].s = true;
					break;
				case T_W:
					S_gSnake[i].x = S_gSnake[i - 1].x + 1;
					S_gSnake[i].y = S_gSnake[i - 1].y;
					S_gSnake[i].Dec = S_gSnake[i - 1].Dec;
					S_gSnake[i].s = true;
					break;
				case T_S:
					S_gSnake[i].y = S_gSnake[i - 1].y - 1;
					S_gSnake[i].x = S_gSnake[i - 1].x;
					S_gSnake[i].Dec = S_gSnake[i - 1].Dec;
					S_gSnake[i].s = true;
					break;
				case T_N:
					S_gSnake[i].y = S_gSnake[i - 1].y + 1;
					S_gSnake[i].x = S_gSnake[i - 1].x;
					S_gSnake[i].Dec = S_gSnake[i - 1].Dec;
					S_gSnake[i].s = true;
					break;
				}
				break;
			}
		}
		SetSnake();
		ProduceFood();

	}
}
