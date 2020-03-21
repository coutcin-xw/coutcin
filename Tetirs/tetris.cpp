#include "tetris.h"

Tetris::Tetris()
{
    srand((unsigned int)time(NULL));    //创建随机数种子

    //初始化 成员变量
    for (int i = 0; i < MAXX; i++)
    {
        for (int j = 0; j < MAXY; j++)
        {
            box[i][j] = 0;
        }
    }

    score = 0;

    for(int i = 0;i < COUNT;i++)
    {
        block.x[i] = -1;
        block.y[i] = -1;
    }
    block.centerX = -1;
    block.centerY = -1;
    block.ID = 0;

    createNextBlock();                  //创建下一个方块

}
void Tetris::createBlock()
{
      for(int i = 0;i < COUNT;i++)
      {
          block.x[i] = nextBlock.x[i];
          block.y[i] = nextBlock.y[i];
      }
       block.centerX = nextBlock.centerX;
       block.centerY = nextBlock.centerY;
       block.ID = nextBlock.ID;

       createNextBlock();
}

void Tetris::createNextBlock()
{
    int centerX = (MAXX-1) / 2;         //创建中心X坐标

    int ID = rand()%7;

    //根据不同的随机数创建方块
    switch (ID)
    {
    case 0:
        //##
        //##
        nextBlock.x[0] = centerX;
        nextBlock.x[1] = centerX;
        nextBlock.x[2] = centerX + 1;
        nextBlock.x[3] = centerX + 1;
        nextBlock.y[0] = -2;
        nextBlock.y[1] = -1;
        nextBlock.y[2] = -2;
        nextBlock.y[3] = -1;
        nextBlock.centerX = 0;
        nextBlock.centerY = 0;
        nextBlock.ID = 0;
        break;
    case 1:
        //####
        //
        nextBlock.x[0] = centerX - 1;
        nextBlock.x[1] = centerX;
        nextBlock.x[2] = centerX + 1;
        nextBlock.x[3] = centerX + 2;
        nextBlock.y[0] = -1;
        nextBlock.y[1] = -1;
        nextBlock.y[2] = -1;
        nextBlock.y[3] = -1;
        nextBlock.centerX = centerX;
        nextBlock.centerY = -1;
        nextBlock.ID = 1;
        break;
    case 2:
        //##
        // ##
        nextBlock.x[0] = centerX - 1;
        nextBlock.x[1] = centerX;
        nextBlock.x[2] = centerX;
        nextBlock.x[3] = centerX + 1;
        nextBlock.y[0] = -2;
        nextBlock.y[1] = -2;
        nextBlock.y[2] = -1;
        nextBlock.y[3] = -1;
        nextBlock.centerX = centerX;
        nextBlock.centerY = -2;
        nextBlock.ID = 2;
        break;
    case 3:
        // ##
        //##
        nextBlock.x[0] = centerX;
        nextBlock.x[1] = centerX + 1;
        nextBlock.x[2] = centerX - 1;
        nextBlock.x[3] = centerX;
        nextBlock.y[0] = -2;
        nextBlock.y[1] = -2;
        nextBlock.y[2] = -1;
        nextBlock.y[3] = -1;
        nextBlock.centerX = centerX;
        nextBlock.centerY = -2;
        nextBlock.ID = 3;
        break;
    case 4:
        //#
        //###
        nextBlock.x[0] = centerX - 1;
        nextBlock.x[1] = centerX - 1;
        nextBlock.x[2] = centerX;
        nextBlock.x[3] = centerX + 1;
        nextBlock.y[0] = -2;
        nextBlock.y[1] = -1;
        nextBlock.y[2] = -1;
        nextBlock.y[3] = -1;
        nextBlock.centerX = centerX;
        nextBlock.centerY = -1;
        nextBlock.ID = 4;
        break;
    case 5:
        //  #
        //###
        nextBlock.x[0] = centerX + 1;
        nextBlock.x[1] = centerX - 1;
        nextBlock.x[2] = centerX;
        nextBlock.x[3] = centerX + 1;
        nextBlock.y[0] = -2;
        nextBlock.y[1] = -1;
        nextBlock.y[2] = -1;
        nextBlock.y[3] = -1;
        nextBlock.centerX = centerX;
        nextBlock.centerY = -1;
        nextBlock.ID = 5;
        break;
    case 6:
        // #
        //###
        nextBlock.x[0] = centerX;
        nextBlock.x[1] = centerX - 1;
        nextBlock.x[2] = centerX;
        nextBlock.x[3] = centerX + 1;
        nextBlock.y[0] = -2;
        nextBlock.y[1] = -1;
        nextBlock.y[2] = -1;
        nextBlock.y[3] = -1;
        nextBlock.centerX = centerX;
        nextBlock.centerY = -1;
        nextBlock.ID = 6;
        break;
    default:
        break;
    }
}

//获得下一个方块
Block Tetris::getNextBlock()
{
    return nextBlock;
}

//获得当前方块
Block Tetris::getBlock()
{
    return block;
}

 //获得当前分数
int Tetris::getScore()
{
    return score;
}

//获得相应方块的状态
int Tetris::getBox(int x,int y)
{
    return box[x][y];
}

//旋转
bool Tetris::rotate()
{
    if(judgeRotatable())
    {
        return true;
    }
    else
    {
        return false;
    }

}

//向左移动
bool Tetris::moveToLeft()
{
    if(move(-1,0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//向右移动
bool Tetris::moveToRight()
{
    if(move(1,0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//向下移动
bool Tetris::moveToDown()
{
    if(!move(0,1))
    {
        blockToBox();
        killLines();

        if(judgeEnd())
        {
            return false;
        }
        else
        {
            createBlock();
        }
    }
    return true;
}

//判断是否结束
bool Tetris::judgeEnd()
{

    for(int i=0;i<MAXX;i++)
    {
        if(box[i][0]==1)
        {
            return true;
        }
    }
    return false;

}

//消去整行
void Tetris::killLines()
{
    int count = 0;      //一次性次消去的行数

    int temp = 0;
     while((temp=getFullLines())!=-1)
     {
         for(int j=temp;j>0;j--)
         {
             for(int i=0;i<MAXX;i++)
             {
                 box[i][j]=box[i][j-1];
             }
         }
         count++;
     }
     //消行奖励分数
     score += count * count *10;

}

//重新初始化
void Tetris::clear()
{
     srand((unsigned int)time(NULL));    //创建随机数种子

    //初始化 成员变量
    for(int i = 0;i < MAXX;i++)
    {
        for(int j = 0;j < MAXY;j++)
        {
            box[i][j] = 0;
        }

    }

    score = 0;                          //初始化分数

    for(int i = 0;i < COUNT;i++)
    {
        block.x[i] = -1;
        block.y[i] = -1;
    }
    block.centerX = -1;
    block.centerY = -1;
    block.ID = 0;

    createNextBlock();                  //创建下一个方块
}

//获得当前窗口宽度
int Tetris::getWidth()
{
    return MAXX * BLOCKW + (MAXX - 1) * INTERVAL;
}

//获得当前窗口高度
int Tetris::getHeight()
{
    return MAXY * BLOCKH + (MAXY -1) * INTERVAL;
}

//获得下一个窗口宽度
int Tetris::getNextWidth()
{
   return NEXTMAXX * BLOCKW + (NEXTMAXX - 1) * INTERVAL;
}

//获得下一个窗口高度
int Tetris::getNextHeight()
{
   return NEXTMAXY * BLOCKH + (NEXTMAXY -1) * INTERVAL;
}


//是否可以移动
//如果可以 则移动 返回ture 否则返回 false
bool Tetris::move(int x,int y)
{
    int newX[COUNT];
    int newY[COUNT];
    int newCenterX;
    int newCenterY;

    for(int i =0;i < COUNT;i++)
    {
        newX[i]=block.x[i] + x;
        newY[i]=block.y[i] + y;

        //判断是否越界 或 不可移动

        if(newX[i] < 0 || newX[i] >= MAXX)
        {
            return false;
        }
        if(newY[i] >= 0 && newY[i] < MAXY)
        {
            if(box[newX[i]][newY[i]] == 1)
            {
                return false;
            }
        }
        else if(newY[i] >= MAXY)
        {
            return false;
        }
    }
    newCenterX=block.centerX + x;
    newCenterY=block.centerY + y;

    for(int i =0;i < COUNT;i++)
    {
        block.x[i]=newX[i];
        block.y[i]=newY[i];
    }
    block.centerX=newCenterX;
    block.centerY=newCenterY;
    return true;
}

//将当前方块放入box中
void Tetris::blockToBox()
{

    for(int i =0;i < COUNT;i++)
    {
        int x = block.x[i];
        int y = block.y[i];
        if(y >= 0)
        {
            box[x][y] = 1;
        }
    }

}

//判断是否可以旋转
bool Tetris::judgeRotatable()
{
    int newX[COUNT];
    int newY[COUNT];
    int newCenterX;
    int newCenterY;

    for(int i =0;i < COUNT;i++)
    {
        int nx = block.x[i] - block.centerX;
        int ny = block.y[i] - block.centerY;
        newX[i] = nx * 0 + ny * (-1) + block.centerX;
        newY[i] = nx * 1 + ny * 0 + block.centerY;


        //判断是否越界 或 不可旋转

        if(newX[i] < 0 || newX[i] >= MAXX)
        {
            return false;
        }
        if(newY[i] > 0 || newY[i] < MAXY)
        {
            if(box[newX[i]][newY[i]] == 1)
            {
                return false;
            }
        }
        else if(newY[i] >= MAXY)
        {
            return false;
        }
    }
    newCenterX=block.centerX;
    newCenterY=block.centerY;

    for(int i =0;i < COUNT;i++)
    {
        block.x[i]=newX[i];
        block.y[i]=newY[i];
    }
    block.centerX=newCenterX;
    block.centerY=newCenterY;
    return true;
}

//获得第一个整行
int Tetris::getFullLines()
{

    int j=0;
    for(j=0;j<MAXY;j++)
    {
        bool judgeMent = true;
        for(int i=0;i<MAXX;i++)
        {
            if(box[i][j]==0)
            {
                judgeMent = false;
            }
        }

        if(judgeMent)
        {
            return j;
        }
    }
    return -1;
}
