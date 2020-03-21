#ifndef NEXTTETRISBOX_H
#define NEXTTETRISBOX_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QBrush>
#include <QColor>

#include "tetris.h"


#define RESTX (MAXX - NEXTMAXX) / 2     //方块x坐标的转换常数
#define RESTY 4

class NextTetrisBox : public QWidget
{
    Q_OBJECT
public:
    explicit NextTetrisBox(QWidget *parent = nullptr);
    void updateNextTetris(Tetris tetris);        //更新视图数据
    void paintEvent(QPaintEvent *event);          //绘制视图
signals:

private:
    Block nextBlock;
};

#endif // NEXTTETRISBOX_H
