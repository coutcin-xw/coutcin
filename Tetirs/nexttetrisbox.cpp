#include "nexttetrisbox.h"

NextTetrisBox::NextTetrisBox(QWidget *parent) : QWidget(parent)
{
    for (int i = 0; i < COUNT; i++)
        {
            nextBlock.x[i] = -1;
            nextBlock.y[i] = -1;
        }
        nextBlock.centerX = -1;
        nextBlock.centerY = -1;
        nextBlock.ID = 0;

        //设置本“下一个”窗口的宽度和高度
        //并设置背景为黑色
        int w = Tetris::getNextWidth();
        int h = Tetris::getNextHeight();
        setFixedSize(w, h);
        setPalette(QPalette(Qt::black));
        setAutoFillBackground(true);
}

//更新视图数据
void NextTetrisBox::updateNextTetris(Tetris tetris)
{
    nextBlock = tetris.getNextBlock();
       for (int i = 0; i < COUNT; i++)
       {
           nextBlock.x[i] -= RESTX;
           nextBlock.y[i] += RESTY;
       }
       //重新绘制
       repaint();
    }

void NextTetrisBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255, 255, 255));
    painter.setPen(pen);
    painter.setBrush(brush);

    //绘制nextBlock中的内容
    for (int i = 0; i < COUNT; i++)
    {
        int x = nextBlock.x[i];
        int y = nextBlock.y[i];
        int x1 = x * BLOCKW + x * INTERVAL;
        int y1 = y * BLOCKH + y * INTERVAL;
        painter.drawRect(x1, y1, BLOCKW, BLOCKH);
    }
}
