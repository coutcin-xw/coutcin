#ifndef TETRIS_H
#define TETRIS_H

#include <ctime>
#include <iostream>

#define MAXX 10         //显示窗口的横向格子数
#define MAXY 20         //显示窗口的纵向格子数
#define NEXTMAXX 6      //“下一个”显示窗口的横向格数
#define NEXTMAXY 6      //“下一个”显示窗口的竖向格数
#define BLOCKW 30       //格子的宽度
#define BLOCKH 30       //格子的高度
#define INTERVAL 4      //单格之间的间隔
#define COUNT 4         //每个方块的小方块数


//定义方块结构体
struct Block
{

    int x[COUNT];       //方块单格x坐标
    int y[COUNT];       //方块单格y坐标
    int centerX;        //方块的中心x坐标
    int centerY;        //方块的中心y坐标
    int ID;             //方块编号
};

//俄罗斯方块类
class Tetris
{
public:
    Tetris();
    void createBlock();         //创建当前方块
    Block getNextBlock();       //获得下一个方块
    Block getBlock();           //获得当前方块
    int getScore();             //获得当前分数
    int getBox(int x,int y);    //获得相应方块的状态
    bool rotate();              //旋转
    bool moveToLeft();          //向左移动
    bool moveToRight();         //向右移动
    bool moveToDown();          //向下移动
    bool judgeEnd();            //判断是否结束
    void killLines();           //消去整行
    void clear();               //重新初始化

    static int getWidth();          //获得当前窗口宽度
    static int getHeight();         //获得当前窗口高度
    static int getNextWidth();      //获得下一个窗口宽度
    static int getNextHeight();     //获得下一个窗口高度


private:
    void createNextBlock();     //创建下一个方块
    bool move(int x,int y);     //是否可以移动
    void blockToBox();          //将当前方块放入box中
    bool judgeRotatable();      //判断是否可以移动
    int getFullLines();         //获得第一个整行

private:
    int box[MAXX][MAXY];        //声明方块显示坐标系数组
    int score;                  //分数
    Block block;                //当前方块
    Block nextBlock;            //下一个方块

};

#endif // TETRIS_H
