#include "Magnifier.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

Magnifier::Magnifier(QWidget *parent) : QWidget(parent, Qt::Window|Qt::FramelessWindowHint|Qt::CustomizeWindowHint)
{
    MagnifierSize = QSize(100,100);
    setFixedSize(MagnifierSize);
    setWindowOpacity(0.5);
    parentWidg = this->parentWidget();
}

Magnifier::~Magnifier()
{
}

void Magnifier::magnifierMove(QPoint *globalMousePos)
{
    this->show();

    QRect desktopRect = QApplication::desktop()->geometry();
    auto newPos = globalMousePos;

    auto newX = newPos->x();
    auto newY = newPos->y();

    auto maxX = desktopRect.width() - MagnifierSize.width();
    auto maxY = desktopRect.height() - MagnifierSize.height();


    if(parentWidg->geometry().right()> this->geometry().left()) newX -= MagnifierSize.width();
    if(parentWidg->geometry().bottom() > this->geometry().bottom()) newY -= MagnifierSize.height();

    if(newX > maxX) newX = maxX;
    if(newY > maxY) newY = maxY;

    if(newX < 0) newX = 0;
    if(newY < 0) newY = 0;

    this->setGeometry(newX,newY,MagnifierSize.width(),MagnifierSize.height());
    this->repaint();
}

void Magnifier::paintEvent(QPaintEvent *event)
{

    QPainter paint(this);

    QWidget *parentWi = this->parentWidget();
    QString sizeStr;
    QTextStream(&sizeStr)<<ParentSize.width()<<" x "<<ParentSize.height();
    paint.drawText(this->rect().bottomLeft(),sizeStr);


}

void Magnifier::SetParentSize(QSize SizeP)
{
    ParentSize = SizeP;
}
