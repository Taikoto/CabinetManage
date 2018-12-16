#include "dialog.h"

#include <QPainter>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);//隐藏标题栏
    setAttribute(Qt::WA_TranslucentBackground, true);//设置背景透明
    //setWindowOpacity(0.95);//设置透明度
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0, 0));//设置画笔颜色
    painter.setBrush(QColor(3, 80, 130, 230));//矩形颜色
    painter.drawRoundedRect(0, 0, width()-1, height()-1, 5, 5);//画圆弧矩形

    QDialog::paintEvent(event);
}
