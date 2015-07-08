#include "Magnifier.h"


Magnifier::Magnifier(QWidget *parent)
    : QWidget(parent, Qt::Window|Qt::FramelessWindowHint|Qt::CustomizeWindowHint|Qt::NoDropShadowWindowHint)
{
    MagnifierSize = QSize(qApp->desktop()->width()/12,qApp->desktop()->width()/12);
    setFixedSize(MagnifierSize);
    setWindowOpacity(1);
    c_simpleWindow = dynamic_cast<SimpleWindow*>(parent);
    desktopPixmap = std::make_shared<QPixmap>(c_simpleWindow->desktopPixmap);

    connect(c_simpleWindow, &SimpleWindow::resizeSimpleWindow, [=](QResizeEvent *event){
        this->ParentSize = event->size();
    });

    this->show();
}

Magnifier::~Magnifier()
{
}

void Magnifier::magnifierMove(QPoint *globalMousePos)
{
    QRect desktopRect = QApplication::desktop()->geometry();
    auto newPos = globalMousePos;

    auto newX = newPos->x();
    auto newY = newPos->y();

    auto maxX = desktopRect.width() - MagnifierSize.width();
    auto maxY = desktopRect.height() - MagnifierSize.height();

    if(c_simpleWindow->geometry().left() == newX) newX -= this->width();
    if(c_simpleWindow->geometry().top() == newY) newY -= this->height();

    if(newX > maxX) newX = maxX;
    if(newY > maxY) newY = maxY;

    if(newX < 0) newX = 0;
    if(newY < 0) newY = 0;

    this->move(newX, newY);
    this->repaint();
}

void Magnifier::paintEvent(QPaintEvent *event)
{

    QPoint scaledCoefficient(this->size().width()/6,this->size().height()/6);
    QRect scaledRect(QCursor::pos()-scaledCoefficient,QCursor::pos()+scaledCoefficient);
    QPixmap zoomPixmap = desktopPixmap->copy(scaledRect).scaled(this->size(), Qt::IgnoreAspectRatio);

    QPainter paint(this);
    QPoint drawPoint = QPoint(0,0);
    paint.drawPixmap(drawPoint, zoomPixmap);

    paint.setPen( QPen(QBrush( QColor(255, 0, 0, 0) ), 2) );
    paint.drawRect(this->rect());
    drawPoint = this->rect().center()/*-QPoint(4,-4)*/;
    paint.drawText(drawPoint,"+");

    QString sizeStr;
    QTextStream(&sizeStr)<<ParentSize.width()<<" x "<<ParentSize.height();

    drawPoint.setX(this->rect().center().x()-23);
    drawPoint.setY(this->rect().bottomLeft().y());
    paint.drawText(drawPoint,sizeStr);
}

