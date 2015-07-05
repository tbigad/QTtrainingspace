#include "Magnifier.h"


Magnifier::Magnifier(QWidget *parent) : QWidget(parent, Qt::Window|Qt::FramelessWindowHint|Qt::CustomizeWindowHint)
{
    MagnifierSize = QSize(100,100);
    setFixedSize(MagnifierSize);
    setWindowOpacity(1);
    parentWidg = parentWidget();
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
    const quint8 zoomSide = 120;

    QPoint zoomStart = QCursor::pos();
    zoomStart -= QPoint(zoomSide / 5, zoomSide / 5);

    QPoint zoomEnd = QCursor::pos();
    zoomEnd += QPoint(zoomSide / 5, zoomSide / 5);

    QRect zoomRect = QRect(zoomStart, zoomEnd);
    zoomRect.setWidth(zoomRect.width());
    zoomRect.setHeight(zoomRect.height());


    QPixmap zoomPixmap = desktopPixmap->copy(zoomRect).scaled(QSize(zoomSide, zoomSide), Qt::KeepAspectRatio);

    QPainter paint(this);
    paint.setPen( QPen(QBrush( QColor(255, 0, 0, 180) ), 2) );

    QPoint drawPoint = QPoint(0,0);
    paint.drawPixmap(drawPoint, zoomPixmap);

    paint.drawRect(this->rect());
    drawPoint = this->rect().center()-QPoint(-5,-15);
    paint.drawText(drawPoint,"+");

    QString sizeStr;
    QTextStream(&sizeStr)<<ParentSize.width()<<" x "<<ParentSize.height();

    drawPoint.setX(this->rect().center().x()-23);
    drawPoint.setY(this->rect().bottomLeft().y());
    paint.drawText(drawPoint,sizeStr);


}

void Magnifier::SetParentSize(QSize SizeP)
{
    ParentSize = SizeP;
}

void Magnifier::SetParentDesktopScreen(QPixmap *primaryScreenPixmap)
{
    desktopPixmap = primaryScreenPixmap;
}
